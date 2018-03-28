/*
** inittrace.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Jan 16 15:21:01 2006 nicolas
** Last update Mon Jan 23 15:16:55 2006 nicolas cormier
*/

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#include "backtrace.h"

extern int	son;
extern int	verbose;
extern int	attach;
extern char*	bin;
extern char*	core;

/*
**
** Init tracer
** Fork or attach
**
*/


/*
**
** Waiting for the traced proc.
** When ready inject breakpoints and launch backtrace loop.
**
*/
void	wait_tracel(void)
{
  int	ret, status, signal;

 while (1)
    {
      ret = wait4(son, &status, 0, NULL);
      if (ret == -1)
	errexit("backtrace: wait4, %s", strerror(errno));
      if(WIFSTOPPED(status))
	{
	  signal = WSTOPSIG(status);
	  switch(signal)
	    {
	    case SIGTRAP:
	    case SIGSTOP:
	      /*
	      ** Ready to inject breakpoints
	      */
	      inject_brkpoints();
	      errno = 0;
	      ptrace(PT_CONTINUE, son, (caddr_t) 1, 0);
	      if (errno)
		errexit("backtrace: ptrace(PTRACE_CONTINUE), %s", strerror(errno));
	      return;
	    default:
	      errno = 0;
	      ptrace(PT_CONTINUE, son, (caddr_t) 1, signal);
	      if (errno)
		errexit("backtrace: ptrace(PTRACE_CONTINUE), %s", strerror(errno));
	    }
	}
      else
	errexit("backtrace: abnormal son exit");
    }

}

void	tracepid(char* pid)
{
  if (!isdigit((int) *pid))
    errexit(USAGE);
  son = atoi(optarg);
  errno = 0;
  ptrace(PT_ATTACH, son, NULL, 0);
  if (errno)
    errexit("backtrace: ptrace(PT_ATTACH, %d), %s", son, strerror(errno));
  attach = 1;
  /*
  ** Waiting traced proc's stop and put breakpoints.
  */
  wait_tracel();
}

void	traceson(char* ubin, char **argv)
{
  bin = ubin;
  if (!(bin = get_bin_path(X_OK)))
    bin = ubin;
  if ((son = fork()) == -1)
    errexit("backtrace: fork, %s", strerror(errno));
  /*
  ** Father exec,
  ** Waiting son's start and put breakpoints.
  */
  if (son)
    {
      wait_tracel();
      return;
    }
  /*
  ** Son exec
  */
  if (setpgid(0, 0) == -1)
    errexit("backtrace: setpgid, %s", strerror(errno));
  errno = 0;
  ptrace(PT_TRACE_ME, 0, (char *) 1, 0);
  if (errno)
    errexit("backtrace: ptrace(PTRACE_TRACEME), %s", strerror(errno));
  execv(bin, argv);
  errexit("backtrace: execl, %s", strerror(errno));
}
