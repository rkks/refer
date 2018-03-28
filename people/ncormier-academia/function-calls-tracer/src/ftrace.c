/*
** ftrace.c for  in /home/nico/lang/c/ftrace/src
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 15:43:44 2006 nicolas
** Last update Wed Apr 26 22:53:03 2006 nicolas
*/

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "ftrace.h"
#include "error.h"
#include "tracee.h"
#include "loop.h"
#include "core.h"

#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/proc/high_layer.h"

#include "api/includes/prototypes/core/low_layer.h"

#include "api/includes/prototypes/prim_types.h"

/*
** Global.
*/
int	verbose = 2;
int	tracer_quit = 0;
/*
** Local.
*/
static tracee_t		tracee;

/*
** atexit.
*/
static void    quit_ftrace(int i)
{
  tracer_quit = i = 1;
  if (!tracee.proc)
    exit(0);
  else if (tracee.status == RUN)
    {
      if (proc_stop(tracee.proc) == -1)
	errexit("ftrace: proc_stop, %s", ftrace_strerror(errno));
    }
}

/*
** If traced process has been attached, detach before quit.
*/
static void	clean_ftrace(void)
{
  if (tracee.proc && tracee.proc->pid != -1 && tracee.proc->attach)
    {
      list_t*	cur;

      /*
      ** Remove user breakpoints.
      */
      for (cur = tracee.brkps; cur; cur = cur->next)
	{
	  if (((brkp_t*) cur->value)->set)
	    {
	      (void) proc_unset_breakpoint(tracee.proc, (brkp_t*) cur->value);
	    }
	  if (cur == tracee.brkps->prev)
	    break;
	}
      /*
      ** Remove ftrace breakpoints.
      */
      for (cur = tracee.fbrkps; cur; cur = cur->next)
	{
	  if (((brkp_t*) cur->value)->set)
	    {
	      (void) proc_unset_breakpoint(tracee.proc, (brkp_t*) cur->value);
	    }
	  if (cur == tracee.fbrkps->prev)
	    break;
	}

      (void) proc_detach(tracee.proc);
    }
}


int	main(int argc, char** argv)
{
  struct sigaction	action;
  int			ch, i, count = 0;
  int			ret, passed = 0;
  addr_t		addr;

  memset(&tracee, 0, sizeof(tracee_t));

  /*
  ** Catch sigint & sigquit
  */
  action.sa_handler = quit_ftrace;
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGQUIT, &action, NULL))
    errexit("ftrace: sigaction(SIGQUIT), %s", ftrace_strerror(errno));
  if (sigaction(SIGINT, &action, NULL))
    errexit("ftrace: sigaction(SIGKILL), %s", ftrace_strerror(errno));

  /*
  ** Count Args Before -p, -e or -c Option
  */
  for (i = 0; i < argc; i++)
    {
      if (!strcmp(argv[i], "-p") ||
	  !strcmp(argv[i], "-e") ||
	  !strcmp(argv[i], "-c")
	  )
	{
	  if (i != argc - 1)
	    argc = i + 2;
	  break;
	}
    }

  if (atexit(clean_ftrace) == -1)
    errexit("ftrace: atexit:, %s", ftrace_strerror(errno));

  for (count = 0; (ch = getopt(argc, argv,"hv:b:p:e:c:")) != -1; count++)
    {
      switch ((char) ch)
	{
	case 'b':
	  /*
	  ** Add Breakpoint
	  */
	  tracee.brkps_toset = list_add(tracee.brkps_toset, optarg);
	  if (tracee.brkps_toset == NULL)
	    errexit("ftrace: list_add:, %s", ftrace_strerror(errno));
	  break;
	case 'p':
	  passed = 1;
	  /*
	  ** Attach To Pid
	  */
	  tracee.proc = proc_new();
	  if (tracee.proc == NULL)
	    errexit("ftrace: proc_new:, %s", ftrace_strerror(errno));
	  if (proc_attach(tracee.proc, atoi(optarg)) == -1)
	    errexit("ftrace: proc_attach:, %s", ftrace_strerror(errno));
	  if (*(argv + i + 2))
	    tracee.proc->path = *(argv + i + 2);
	  else
	    tracee.proc->path = proc_get_bin_path(tracee.proc);
	  break;
	case 'e':
	  passed = 1;
	  /*
	  ** Exec Process
	  */
	  tracee.proc = proc_new();
	  if (tracee.proc == NULL)
	    errexit("ftrace: proc_new:, %s", ftrace_strerror(errno));
	  if (proc_create(tracee.proc, optarg, argv + i + 1) == -1)
	    errexit("ftrace: proc_create:, %s", ftrace_strerror(errno));
	  break;
	case 'c':
	  passed = 1;
	  /*
	  ** Backtrace Core
	  */
	  core_trace(optarg, *(argv + i + 2));
	  return (0);
	  break;
	case 'v':
	  verbose = atoi(optarg);
	  if (verbose > 0 && verbose < 5)
	    break;
	case 'h':
	default:
	  errexit(USAGE);
	  break;
	}
      if (passed == 1)
	break;
    }

  if (!passed)
    errexit(USAGE);

  /*
  ** Tracee Ready !
  */
  ret = init_loop(&tracee, &addr);
  if (tracer_quit)
    return (0);
  switch (ret)
    {
    case TRACEE_OK:
      if (tracing_loop(&tracee, addr) == -1)
	errexit("ftrace: tracing_loop:, %s", ftrace_strerror(errno));
      break;
    case TRACEE_EXIT:
      return (0);
    default:
      errexit("ftrace: init_loop:, %s", ftrace_strerror(errno));
    }

  return (0);
}
