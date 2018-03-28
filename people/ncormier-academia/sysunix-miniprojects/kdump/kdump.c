/*
** kdump.c for  in /home/nico/lang/c
**
** Made by Nicolas Cormier
** Login   <nico@epita.fr>
**
** Started on  Wed Oct 26 18:28:17 2005 Nicolas Cormier
** Last update Wed Nov  2 13:34:39 2005 nicolas cormier
*/

#include <sys/param.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/ktrace.h>

#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "kdump.h"

/* opt.c */
void	opt_parse(int argc, char **argv, int *noarg_opt, int *trpoints, char **arg_opt);

/* dump_func.c */
void	dump_type(struct ktr_header *header);
void	dump_time(struct ktr_header *header, int *opt);
void	dump_rest(struct ktr_header *ktrheader_buf, void* ktr_buf, int *opt, char **arg_opt);

static void	errexit(char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s\n", buf);
  va_end(ap);
  exit(1);
}

static int	is_trpoints(struct ktr_header *ktrheader, int *trpoints)
{
  switch (ktrheader->ktr_type)
    {
    case KTR_CSW:
      if ((*trpoints & MASKOPT_TR_W) == MASKOPT_TR_W)
	return (1);
      break;
    case KTR_PSIG:
      if ((*trpoints & MASKOPT_TR_S) == MASKOPT_TR_S)
	return (1);
      break;
    case KTR_USER:
      if ((*trpoints & MASKOPT_TR_U) == MASKOPT_TR_U)
	return (1);
      break;
    case KTR_GENIO:
      if ((*trpoints & MASKOPT_TR_I) == MASKOPT_TR_I)
	return (1);
      break;
    case KTR_NAMEI:
      if ((*trpoints & MASKOPT_TR_N) == MASKOPT_TR_N)
	return (1);
      break;
    case KTR_SYSRET:
      if ((*trpoints & MASKOPT_TR_C) == MASKOPT_TR_C)
	return (1);
      break;
    case KTR_SYSCALL:
      if ((*trpoints & MASKOPT_TR_C) == MASKOPT_TR_C)
	return (1);
      break;
    }
  return (0);
}

int			main(int argc, char **argv)
{
  FILE*			tracestream = 0x0;
  char*			arg_opt[3] = {DEFAULT_TRACEFILE, 0x0, 0x0};
  int			noarg_opt = 0;
  int			trpoints = MASKOPT_TR_DEF;
  struct ktr_header	ktrheader_buf;
  int			nbread;
  void*			ktr_buf;
  int			ktr_bufsize = 0;
  int			print_it = 0;

  if (!(ktr_buf = malloc(1024)))
    errexit("%s: %s", argv[0], "Malloc Error");
  opt_parse(argc, argv, &noarg_opt, &trpoints, arg_opt);
  if (!(tracestream = fopen(arg_opt[TABIDX_FILE], "r")))
    errexit("%s: %s: %s", argv[0], arg_opt[TABIDX_FILE], strerror(errno));
  while (1)
    {
      nbread = fread(&ktrheader_buf, 1, sizeof(struct ktr_header), tracestream);
      if (nbread == 0 && (noarg_opt & MASKOPT_LOOPREADING) == MASKOPT_LOOPREADING)
	{
	  sleep(1);
	  clearerr(tracestream);
	  continue;
	}
      else if (nbread <= 0)
	break;
      if (nbread != sizeof(struct ktr_header))
	errexit("%s: %s", argv[0], "Bad Header");
      print_it = is_trpoints(&ktrheader_buf, &trpoints);
      if (print_it)
	printf("  %d %s\t", ktrheader_buf.ktr_pid, ktrheader_buf.ktr_comm);
      if (ktr_bufsize < ktrheader_buf.ktr_len)
	{
	  if (!(ktr_buf = realloc(ktr_buf, ktrheader_buf.ktr_len)))
	    errexit("%s: %s", argv[0], "Malloc Error");
	  ktr_bufsize = ktrheader_buf.ktr_len;
	}
      nbread = fread(ktr_buf, 1, ktrheader_buf.ktr_len, tracestream);
      if (nbread != ktrheader_buf.ktr_len)
	errexit("%s: %s", argv[0], "Bad Len");
      if (!print_it)
	continue;
      dump_time(&ktrheader_buf, &noarg_opt);
      dump_type(&ktrheader_buf);
      dump_rest(&ktrheader_buf, ktr_buf, &noarg_opt, arg_opt);
      printf("\n");
    }
  return (0);
}



