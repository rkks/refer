/*
** strace.c for  in /home/nico/lang/c/strace
**
** Made by Nicolas Cormier
** Login   <nico@epita.fr>
**
** Started on  Wed Dec  7 16:06:05 2005 Nicolas Cormier
** Last update Thu Jan  5 13:09:46 2006 nicolas cormier
*/

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <machine/reg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>

#include "strace.h"
#include "syscalls.h"

void	print_param(int param, char* param_type);

int	son = -1;
int	detach = 0;
int	verbose = 0;

static int	get_sizeof(char* str)
{
  int		i, limit;

  limit = sizeof(params_size) / sizeof(param_size_t);
  for (i = 0; i < limit; i++)
    if (!strcmp(params_size[i].str, str))
      return (params_size[i].size);
  return (sizeof(int));
}

static void	detach_h(int t)
{
  t = 0;
  detach = 1;
}

static void	tracepid(char* pid)
{
  struct sigaction	action;

  if (!isdigit((int) *pid))
    errexit(USAGE);
  son = atoi(optarg);
  errno = 0;
  ptrace(PT_ATTACH, son, NULL, 0);
  if (errno)
    errexit("strace: ptrace(PT_ATTACH), %s", strerror(errno));
  action.sa_handler = detach_h;
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &action, NULL))
    errexit("strace: sigaction(SIGINT), %s", strerror(errno));
}

static void	traceson(char* bin, char **argv)
{
  if ((son = fork()) == -1)
    errexit("strace: fork, %s", strerror(errno));
  if (son)
    return;
  errno = 0;
  ptrace(PT_TRACE_ME, 0, (char *) 1, 0);
  if (errno)
    errexit("strace: ptrace(PTRACE_TRACEME), %s", strerror(errno));
  execvp(bin, argv);
  errexit("strace: execl, %s", strerror(errno));
}

static void	print_params(syscnfo_t** last, struct reg* cur_reg, caddr_t* cur)
{
  int		ret, i;

  for (i = 0; (*last)->args_type[i]; i++)
    {
      *cur += get_sizeof((*last)->args_type[i]);
      errno = 0;
      ret = ptrace(PT_READ_D, son, *cur, 0);
      if (errno)
	errexit("strace: ptrace(PT_READ_D), %s", strerror(errno));
      print_param(ret, (*last)->args_type[i]);
      if ((*last)->args_type[i + 1])
	printf(", ");
    }
  printf(")");
  if (strcmp((*last)->ret_type, "void") && strcmp((*last)->name, "nosys"))
    {
      if (cur_reg->r_eflags & 1 && verbose)
	printf("\tSyscall Failed, error code = %d (\"%s\")", cur_reg->r_eax, strerror(cur_reg->r_eax));
      else
	{
	  printf(" = ");
	  print_param(cur_reg->r_eax, (*last)->ret_type);
	}
    }
  printf("\n");
}

static void	print_syscall(syscnfo_t** last, struct reg* cur_reg, caddr_t* cur)
{
  int		ret;

  *cur = (caddr_t) cur_reg->r_esp;
  *last = &(syscalls_info[cur_reg->r_eax]);
  if (!strcmp(syscalls_info[cur_reg->r_eax].name, "syscall") ||
      !strcmp(syscalls_info[cur_reg->r_eax].name, "__syscall"))
    {
      *cur += get_sizeof(syscalls_info[cur_reg->r_eax].args_type[0]);
      errno = 0;
      ret = ptrace(PT_READ_D, son, *cur, 0);
      if (errno)
	errexit("strace: ptrace(PT_READ_D), %s", strerror(errno));
      printf("%s(", syscalls_info[ret].name);
      *last = &(syscalls_info[ret]);
      *cur += get_sizeof(syscalls_info[cur_reg->r_eax].args_type[0]);
    }
  else
    printf("%s(", syscalls_info[cur_reg->r_eax].name);
  if (!strcmp(syscalls_info[cur_reg->r_eax].name, "exit"))
    print_params(last, cur_reg, cur);
}

static int	sigtreat(int status)
{
  int		signal;

  if(WIFSTOPPED(status))
    {
      if (detach)
	{
	  errno = 0;
	  ptrace(PT_DETACH, son, (char *) 1, 0);
	  if (errno)
	    errexit("strace: ptrace(PT_DETACH), %s", strerror(errno));
	  return (-1);
	}
      signal = WSTOPSIG(status);
      switch(signal)
	{
	case SIGTRAP:
	  return (0);
	  break;
	default:
	  printf("SIGNAL RECEIVED: %d (\"%s\")\n", signal, sys_siglist[signal]);
	  errno = 0;
	  ptrace(PT_STEP, son, (caddr_t) 1, signal);
	  if (errno)
	    errexit("strace: ptrace(PTRACE_STEP), %s", strerror(errno));
	  return (1);
	}
    }
  return (-1);
}

static void	father_exec(void)
{
  int		status, ret;
  syscnfo_t*	last = NULL;
  struct reg	cur_reg;
  caddr_t	cur;

  while (1)
    {
      ret = wait4(son, &status, 0, NULL);
      if (ret == -1)
	errexit("strace: wait4, %s", strerror(errno));
      if ((ret = sigtreat(status)) == -1)
	break;
      else if (ret)
	continue;
      errno = 0;
      ptrace(PT_GETREGS, son, (caddr_t) &cur_reg, 0);
      if (errno)
	errexit("strace: ptrace(PT_GETREGS), %s", strerror(errno));
      errno = 0;
      ret = ptrace(PT_READ_I, son, (caddr_t) cur_reg.r_eip, 0);
      if (errno)
	errexit("strace: ptrace(PT_READ_I), %s", strerror(errno));
      if (last)
	{
	  print_params(&last, &cur_reg, &cur);
	  last = NULL;
	}
      if ((0x000000FF & ret) == 0xCD && (0x0000FF00 & ret) >> 8 ==  0x80)
	{
	  if ((unsigned int) cur_reg.r_eax > (sizeof(syscalls_info)/sizeof(syscnfo_t)))
	    printf(":o)\n");
	  else
	    print_syscall(&last, &cur_reg, &cur);
	}
      errno = 0;
      ptrace(PT_STEP, son, (caddr_t) 1, 0);
      if (errno)
	errexit("strace: ptrace(PT_STEP), %s", strerror(errno));
    }
}

int	main(int argc, char* argv[])
{
  int	ch, i, count = 0;
  int	passed = 0;

  for (i = 0; i < argc; i++)
    if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "-e"))
      {
	if (i != argc - 1)
	  argc = i + 2;
	break;
      }
  while ((ch = getopt(argc, argv,"hp:e:v")) != EOF)
    {
      if (passed == 1)
	break;
       switch ((char) ch)
	{
	case 'p':
	  passed = 1;
	  tracepid(optarg);
	  break;
	case 'e':
	  passed = 1;
	  traceson(optarg, argv + count + 2);
	  break;
	case 'v':
	  verbose = 1;
	  break;
	case 'h':
	  errexit(USAGE);
	  break;
	default:
	  errexit(USAGE);
	  break;
	}
       count++;
    }
  if (son == -1)
    errexit(USAGE);
  father_exec();
  printf("\n");
  return (0);
}
