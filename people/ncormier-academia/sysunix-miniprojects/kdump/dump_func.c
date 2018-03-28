/*
** dump_func.c for  in /home/nico/lang/c/kdump
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Fri Oct 28 21:59:03 2005 nicolas
** Last update Wed Nov  2 14:17:37 2005 Nicolas Cormier
*/

#include <sys/param.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/ktrace.h>
#include <sys/ioctl.h>

#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <vis.h>

#ifdef __APPLE__
# define ERESTART	-1
# define EJUSTRETURN	-2

# include <sys/syscall.h>
# include "darwin/ksyscalls.h"
# include "darwin/kioctlname.c"

#elif __FreeBSD__
# define _KERNEL
# include <sys/errno.h>
# undef _KERNEL

# include "freebsd/ksyscalls.h"
# include "freebsd/kioctlname.c"
#endif


#include "kdump.h"

/* timeval.c */
void	timeval_diff(struct timeval *start, struct timeval *end, struct timeval *diff);
void	timeval_set(struct timeval *t, long sec, long usec);

static void	dump_csw(struct ktr_csw *csw);
static void	dump_psig(struct ktr_psig *psig);
static void	dump_user(char *buf, int buflen, int *opt);
static void	dump_genio(struct ktr_genio *genio, int buflen, int *opt, char **arg_opt);
static void	dump_namei(char *buf, int buflen);
static void	dump_sysret(struct ktr_sysret *sysret, int *opt);
static void	dump_syscall(struct ktr_syscall *syscall, int *opt);

void	dump_rest(struct ktr_header *ktrheader_buf, void* ktr_buf, int *opt, char **arg_opt)
{
  switch (ktrheader_buf->ktr_type)
    {
    case KTR_CSW:
      dump_csw((struct ktr_csw *) ktr_buf);
      break;
    case KTR_PSIG:
      dump_psig((struct ktr_psig *) ktr_buf);
      break;
    case KTR_USER:
      dump_user((char *) ktr_buf, ktrheader_buf->ktr_len, opt);
      break;
    case KTR_GENIO:
      dump_genio((struct ktr_genio *) ktr_buf, ktrheader_buf->ktr_len, opt, arg_opt);
      break;
    case KTR_NAMEI:
      dump_namei(ktr_buf, ktrheader_buf->ktr_len);
      break;
    case KTR_SYSRET:
      dump_sysret((struct ktr_sysret *) ktr_buf, opt);
      break;
    case KTR_SYSCALL:
      dump_syscall((struct ktr_syscall *) ktr_buf, opt);
      break;
    }
}

static void	dump_csw(struct ktr_csw *csw)
{
  char*		out = "resume";
  char*		user = "kernel";

  if (csw->out)
    out = "stop";
  if (csw->user)
    user = "user";
  printf("%s %s", out, user);
}

static void	dump_psig(struct ktr_psig *psig)
{
  printf("%s ", sys_signame[psig->signo]);
  if (psig->action == SIG_DFL)
    printf("SIG_DFL");
  else
    {
      printf("caught handler=0x%lx mask=0x%x code=0x%x\n",
#ifdef __FreeBSD__
	     (u_long) psig->action, psig->mask.__bits[0], psig->code);
#else
	     (u_long) psig->action, psig->mask, psig->code);
#endif
    }
}

static void	dump_user(char *buf, int buflen, int *opt)
{
  printf("%d ", buflen);
  for (; buflen--; buf++)
    if ((*opt & MASKOPT_DECIMAL) == MASKOPT_DECIMAL)
      printf(" %d", *buf);
    else
      printf(" %02x", *buf);
}

static int		dump_genio_get_width(int *opt)
{
  struct winsize	ws;

  if ((*opt & MASKOPT_HOCSUPPR) == MASKOPT_HOCSUPPR)
    return (DEFAULT_LINE_WIDTH);
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
    return (DEFAULT_LINE_WIDTH);
  return (ws.ws_col);
}

static void	dump_genio_data_hexa(int restlen, char *data, int *opt)
{
  int		nbwrite = 0;
  int		nbrhex = dump_genio_get_width(opt);
  int		totlen;
  int		count;

  nbrhex -= strlen("       ");
  nbrhex -= strlen("0x0000 ");
  nbrhex -= strlen(" ||");
  if (nbrhex <= 0)
    return;
  nbrhex /= 7;
  for (totlen = 0; totlen < restlen; totlen += nbrhex * 2)
    {
      printf("       0x%04x ", totlen);
      for (count = totlen; count < restlen && nbwrite != nbrhex * 2; count++, nbwrite++)
	{
	  printf("%02x", data[count] & 0xff);
	  if (nbwrite % 2)
	    printf(" ");
	}
      for (; nbwrite != nbrhex * 2; nbwrite++)
	{
	  printf("  ");
	  if (nbwrite % 2)
	    printf(" ");
	}
      printf(" |");
      nbwrite = 0;
      for (count = totlen; count < restlen && nbwrite != nbrhex * 2; count++, nbwrite++)
	{
	  if (data[count] >= ' ' && data[count] <= '~')
	    printf("%c", data[count]);
	  else
	    printf(".");
	}
      printf("|");
      if (totlen + nbrhex * 2 < restlen)
	printf("\n");
      nbwrite = 0;
    }
}

static void	dump_genio_data_ascii(int restlen, char *data, int *opt)
{
  char		buf[BUFSIZ];
  char		*ptr;
  int		nbwrite = 0;
  int		width = dump_genio_get_width(opt);

  nbwrite += printf("       \"");
  for (; restlen; restlen--, data++)
    {
      if (nbwrite == width - 1)
	nbwrite = printf("\\\n       ");
      vis(buf, *data, VIS_CSTYLE, *(data + 1));
      for (ptr = buf; *ptr; ptr++)
	{
	  if (*ptr == '\n')
	      nbwrite = printf("\n        ");
	  else
	      nbwrite += printf("%c", *ptr);
	}
    }
  printf("\"");
}


static void	dump_genio(struct ktr_genio *genio, int buflen, int *opt, char **arg_opt)
{
  int		restlen = buflen - sizeof(struct ktr_genio);
  char*		data = (char *) genio + sizeof(struct ktr_genio);
  char*		action = "wrote";
  int		hexa = 0;
  int		count = 0;

  if (genio->ktr_rw == UIO_READ)
    action = "read";
  printf("fd %d %s %d bytes\n", genio->ktr_fd, action, restlen);
  if (arg_opt[TABIDX_MAXDATA] && atoi(arg_opt[TABIDX_MAXDATA]) < restlen)
    if ((restlen = atoi(arg_opt[TABIDX_MAXDATA])) <= 0)
      restlen = buflen - sizeof(struct ktr_genio);
  for (count = 0; count < restlen; count++)
    {
      if (
	  (data[count] >= 32 && data[count] < 127) ||
	  data[count] == 0  || data[count] == 9   ||
	  data[count] == 10 || data[count] == 13
	  )
	continue;
      hexa = 1;
      break;
    }
  if (hexa)
    dump_genio_data_hexa(restlen, data, opt);
  else
    dump_genio_data_ascii(restlen, data, opt);
}

static void	dump_namei(char *buf, int buflen)
{
  printf("\"%.*s\"", buflen, buf);
}

static void	dump_sysret(struct ktr_sysret *sysret, int *opt)
{
  if (sysret->ktr_code >= K_SYSMAX_SYSCALLS || sysret->ktr_code < 0)
    printf("[%d] ", sysret->ktr_code);
  else
    printf("%s ", gl_syscallname[sysret->ktr_code]);
  if (!sysret->ktr_error)
    {
      if ((*opt & MASKOPT_HOCSUPPR) == MASKOPT_HOCSUPPR)
	{
	  if ((*opt & MASKOPT_DECIMAL) == MASKOPT_DECIMAL)
	    printf("%ld", (long) sysret->ktr_retval);
	  else
	    printf("%#lx", (long) sysret->ktr_retval);
	}
      else
	{
	  printf("%d", sysret->ktr_retval);
	  if (sysret->ktr_retval < 0 || sysret->ktr_retval > 9)
	    printf("/%#lx", (long) sysret->ktr_retval);
	}
    }
  else if (sysret->ktr_error == ERESTART)
    printf("RESTART");
  else if (sysret->ktr_error == EJUSTRETURN)
    printf("JUSTRETURN");
  else
    {
      printf("-1 errno %d", sysret->ktr_error);
      if (!((*opt & MASKOPT_HOCSUPPR) == MASKOPT_HOCSUPPR))
	printf(" %s", strerror(sysret->ktr_error));
    }
}

static void	dump_syscall(struct ktr_syscall *syscall, int *opt)
{
  int		count = 0;
  char		sym = '(';

  if (syscall->ktr_code >= K_SYSMAX_SYSCALLS || syscall->ktr_code < 0)
    printf("[%d]", syscall->ktr_code);
  else
    printf("%s", gl_syscallname[syscall->ktr_code]);
  if (!((*opt & MASKOPT_HOCSUPPR) == MASKOPT_HOCSUPPR))
    {
      if (syscall->ktr_code == SYS_ioctl)
	{
	  if ((*opt & MASKOPT_DECIMAL) == MASKOPT_DECIMAL)
	    printf("(%ld", (long) *(syscall->ktr_args + count++));
	  else
	    printf("(%#lx", (long) *(syscall->ktr_args + count++));
	  if (ioctlname(*(syscall->ktr_args + count)) != NULL)
	    printf(",%s", ioctlname(*(syscall->ktr_args + count)));
	  else
	    {
	      if ((*opt & MASKOPT_DECIMAL) == MASKOPT_DECIMAL)
		printf(",%ld", (long) *(syscall->ktr_args + count));
	      else
		printf(",%#lx ", (long) *(syscall->ktr_args + count));
	    }
	  count++;
	  sym = ',';
	}
      else if (syscall->ktr_code == SYS_ptrace)
	{
	  ;
	}
    }
  for (; count < syscall->ktr_narg; count++)
    {
      if ((*opt & MASKOPT_DECIMAL) == MASKOPT_DECIMAL)
	printf("%c%ld", sym, (long) *(syscall->ktr_args + count));
      else
	printf("%c%#lx", sym, (long) *(syscall->ktr_args + count));
      if (sym == '(')
	sym = ',';
      if (count == syscall->ktr_narg - 1)
	printf(")");
    }
}

void	dump_type(struct ktr_header *header)
{
  switch (header->ktr_type)
    {
    case KTR_CSW:
      printf("CSW   ");
      break;
    case KTR_PSIG:
      printf("PSIG  ");
      break;
    case KTR_USER:
      printf("USER  ");
      break;
    case KTR_SYSCALL:
      printf("CALL  ");
      break;
    case KTR_SYSRET:
      printf("RET   ");
      break;
    case KTR_NAMEI:
      printf("NAMI  ");
      break;
    case KTR_GENIO:
      printf("GIO   ");
      break;
    default:
      ;
    }
}

static struct timeval	last_timestamps = {.tv_sec = 0, .tv_usec = 0};
void		dump_time(struct ktr_header *header, int *opt)
{
  struct timeval	temp;

  if (last_timestamps.tv_sec == 0)
    last_timestamps = header->ktr_time;
  if ((*opt & MASKOPT_TIMESTAMPS) == MASKOPT_TIMESTAMPS)
    {
      timeval_diff(&last_timestamps, &(header->ktr_time), &temp);
#ifdef __APPLE__
      printf("%ld.%06ld ", (long int) temp.tv_sec, (long int) temp.tv_usec);
#else
      printf("%ld.%06ld ", temp.tv_sec, temp.tv_usec);
#endif
    }
  else if ((*opt & MASKOPT_RETIMESTAMPS) == MASKOPT_RETIMESTAMPS)
    {
      timeval_diff(&last_timestamps, &(header->ktr_time), &temp);
      timeval_set(&last_timestamps, header->ktr_time.tv_sec, header->ktr_time.tv_usec);
#ifdef __APPLE__
      printf("%ld.%06ld ", (long int) temp.tv_sec, (long int) temp.tv_usec);
#else
      printf("%ld.%06ld ", temp.tv_sec, temp.tv_usec);
#endif
    }
  else if ((*opt & MASKOPT_ABSTIMESTAMPS) == MASKOPT_ABSTIMESTAMPS)
#ifdef __APPLE__
    printf("%ld.%06ld ", (long int) header->ktr_time.tv_sec, (long int) header->ktr_time.tv_usec);
#else
    printf("%ld.%06ld ", header->ktr_time.tv_sec, header->ktr_time.tv_usec);
#endif
}
