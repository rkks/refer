/*
** loop.c for  in /home/nico/lang/c/ftrace/src
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Sun Apr  2 19:25:07 2006 nicolas
** Last update Thu Apr 27 13:08:13 2006 nicolas
*/

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "ftrace.h"
#include "error.h"
#include "warning.h"
#include "tracee.h"
#include "loop.h"
#include "bin.h"
#include "brkps.h"
#include "verbose.h"

#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/proc/high_layer.h"

#include "api/includes/prototypes/bin/low_layer.h"
#include "api/includes/prototypes/bin/high_layer/bin.h"
#include "api/includes/prototypes/bin/high_layer/symbols.h"
#include "api/includes/prototypes/bin/high_layer/debug.h"

#include "api/includes/prototypes/prim_types.h"

/*
** Global.
*/

extern int	tracer_quit;

int	init_loop(tracee_t* tracee, addr_t* will_start)
{
  int		signal, ret, brk_set = 0;
  list_t*	cur;
  brkp_t*	brkp;

  /*
  ** Address where the tracing will start.
  */
  *will_start = 0;
  /*
  ** Init binaries.
  */
  tracee->binh = bin_create_handler(tracee->proc->path);
  if (tracee->binh == NULL)
    errexit("ftrace: bin_create_handler, %s", ftrace_strerror(errno));
  bin_refresh_depends_list(proc_get_depends_list(tracee->proc), tracee->binh);
  /*
  ** Inject breakpoints.
  */
  brkps_inject(tracee);
  /*
  ** Waiting for breakpoint ?
  */
  for (cur = tracee->brkps; cur; cur = cur->next)
    {
      if (((brkp_t*) cur->value)->set)
	{
	  brk_set = 1;
	  break;
	}
      if (cur == tracee->brkps->prev)
	break;
    }
  /*
  ** No breakpoint setted, trace !
  */
  if (!brk_set && !tracee->brkps_toset)
    {
      /*
      ** Remove FTrace breakpoints
      */
      for (cur = tracee->fbrkps; cur; cur = cur->next)
	{
	  if (((brkp_t*) cur->value)->set)
	    {
	      if (proc_unset_breakpoint(tracee->proc, (brkp_t*) cur->value) == -1)
		errexit("ftrace: proc_unset_breakpoint, %s", ftrace_strerror(errno));
	    }
	  if (cur == tracee->fbrkps->prev)
	    break;
	}
      return (TRACEE_OK);
    }
  /*
  ** Else wait for breakpoint hit.
  */
  tracee->status = RUN;
  if (proc_continue(tracee->proc, 0) == -1)
    errexit("ftrace: proc_continue, %s", ftrace_strerror(errno));
  while (1)
    {
      ret = proc_wait_debug_event(tracee->proc, &signal);
      if (ret == -1)
	errexit("ftrace: proc_wait_debug_event, %s", ftrace_strerror(errno));
      switch (ret)
	{
	case DEBUG_EVENT:
	  if (tracer_quit)
	    return (0);
	  /*
	  ** if ftrace breakpoint:
	  */
	  brkp = proc_seek_brkps(tracee->proc, tracee->fbrkps);
	  if (brkp && brkp->set)
	    {
	      /*
	      ** Lookup if any shared objects has been load.
	      ** If there is a new so, try de resolve unsolved user breakpoint.
	      */
	      if (bin_refresh_depends_list(proc_get_depends_list(tracee->proc), tracee->binh))
		brkps_try_resolved(tracee);
	      /*
	      ** Unset ftrace one shot breakpoint
	      */
	      if (proc_unset_breakpoint(tracee->proc, brkp) == -1)
		errexit("ftrace: proc_unset_breakpoint, %s", ftrace_strerror(errno));
	      /*
	      ** Step back after restore and continue to seek user breakpoint.
	      */
	      if (proc_step_addr(tracee->proc, brkp->addr, 0) == -1)
		errexit("ftrace: proc_step_addr, %s", ftrace_strerror(errno));
	      break;
	    }
	  /*
	  ** if user breakpoint:
	  */
	  brkp = proc_seek_brkps(tracee->proc, tracee->brkps);
	  if (brkp && brkp->set)
	    {
	      /*
	      ** Breakpoint hit!
	      ** Remove every breakpoints and start tracing !
	      */
	      /*
	      ** Remove user breakpoints.
	      */
	      for (cur = tracee->brkps; cur; cur = cur->next)
		{
		  if (((brkp_t*) cur->value)->set)
		    {
		      if (proc_unset_breakpoint(tracee->proc, (brkp_t*) cur->value) == -1)
			errexit("ftrace: proc_unset_breakpoint, %s", ftrace_strerror(errno));
		    }
		  if (cur == tracee->brkps->prev)
		    break;
		}
	      /*
	      ** Remove ftrace breakpoints.
	      */
	      for (cur = tracee->fbrkps; cur; cur = cur->next)
		{
		  if (((brkp_t*) cur->value)->set)
		    {
		      if (proc_unset_breakpoint(tracee->proc, (brkp_t*) cur->value) == -1)
			errexit("ftrace: proc_unset_breakpoint, %s", ftrace_strerror(errno));
		    }
		  if (cur == tracee->fbrkps->prev)
		    break;
		}
	      /*
	      ** Step back after restore.
	      */
	      if (proc_step_addr(tracee->proc, brkp->addr, 0) == -1)
		errexit("ftrace: proc_step_addr, %s", ftrace_strerror(errno));
	      *will_start = brkp->addr;
	      return (TRACEE_OK);
	    }
	  if (proc_continue(tracee->proc, 0) == -1)
	    errexit("ftrace: proc_continue, %s", ftrace_strerror(errno));
	  break;
	case SIGNAL_EVENT:
	  pwarn("Program received signal %d, %s\n", signal, sys_siglist[signal]);
	  if (proc_continue(tracee->proc, signal) == -1)
	    errexit("ftrace: proc_continue, %s", ftrace_strerror(errno));
	  break;
	case EXIT_EVENT:
	  pverbose(1, "\nProgram exited normally.\n");
	  return (TRACEE_EXIT);
	  break;
	case EXIT_SIGNALED_EVENT:
	  pverbose(1, "Program exited with signal %d, %s\n", signal, sys_siglist[signal]);
	  return (TRACEE_EXIT);
	  break;
	default:
	  errexit("ftrace: proc_wait_debug_event, Unknow Error !");
	}
    }
  return (-1);
}

static int	nbrtab = 0;
/*
** Save info about type of call return.
*/
static list_t*	return_debuginfo_list = NULL;

static void	print_binsym(tracee_t* tracee, bin_t* bin, addr_t addr, int type)
{
  sym_t*	sym;
  debug_t*	debug;
  list_t*	list = NULL;
  arg_t*	arg;
  int		i;

  if (bin)
    {
      sym = sym_get_frm_addr(bin->syms, addr, EXACT);
      if (sym)
	{
	  pverbose(2, " (%s)", sym->name);
	  if (type == EXTCALL)
	    pverbose(2, " @ EXT");
	  else if (type == PLTCALL)
	    pverbose(2, " @ PLT");
	  debug = debug_get_frm_addr(bin->debugs, sym->addr);
	  if (debug)
	    {
	      pverbose(2, " @ %s:%d", debug->file, debug->line);
	      if (proc_seek_args(tracee->proc, debug->args) != -1)
		{
		  pverbose(3, "\n");
		  for (i = 0; i < nbrtab; i++)
		    pverbose(3, "\t|");

		  if (debug->ret)
		    pverbose(3, " %s	%s(\n", debug->ret->type, debug->name);
		  else
		    pverbose(3, " ??	%s(\n", debug->name);
		  for (list = debug->args->prev; list; list = list->prev)
		    {
		      for (i = 0; i < nbrtab; i++)
			pverbose(3, "\t|");

		      arg = (arg_t*) list->value;
		      pverbose(3, "\t%s\t%s = %s;\n", arg->type, arg->name, arg->value);
		      if (list == debug->args)
			break;
		    }
		  for (i = 0; i < nbrtab; i++)
		    pverbose(3, "\t|");
		  pverbose(3, " );");
		}
	      return_debuginfo_list = list_add(return_debuginfo_list, (void*) debug->ret);
	    }
	  return;
	}
    }
  pverbose(2, " (??)");
  if (type == EXTCALL)
    pverbose(2, " @ EXT");
  else if (type == PLTCALL)
    pverbose(2, " @ PLT");
  return_debuginfo_list = list_add(return_debuginfo_list, NULL);
}

static void	print_call(tracee_t* tracee, int ret, addr_t addr)
{
  bin_t*	bin;
  int		i;
  arg_t*	return_info;

  if (ret > 0)
    {
      if (ret == CALL || ret == EXTCALL || ret == PLTCALL)
	{
	  nbrtab++;

	  /*
	  ** Try to get binary which contains the addr.
	  ** Reload depends if not found and try again.
	  */
	  bin = bin_find(tracee->binh, addr);
	  if (!bin)
	    {
	      bin_refresh_depends_list(proc_get_depends_list(tracee->proc),
				       tracee->binh);
	      bin = bin_find(tracee->binh, addr);
	    }

	  for (i = 0; i < nbrtab; i++)
	    pverbose(1, "\t|");
#ifdef __USE_LONG_ADDR_T__
	  pverbose(1, " 0x%lx", addr);
#else
	  pverbose(1, " 0x%x", addr);
#endif /* __USE_LONG_ADDR_T__ */
	  print_binsym(tracee, bin, addr, ret);
	  pverbose(1, "\n");

	  if (ret == EXTCALL)
	    nbrtab--;
	}
      else if (ret == RET)
	{
	  /*
	  ** HAN !
	  */
	  if (nbrtab > 0)
	    {
	      /*
	      ** Print ret return.
	      */
	      if (return_debuginfo_list)
		{
		  return_info = (arg_t*) return_debuginfo_list->value;
		  if (return_info)
		    {
		      if (proc_seek_return_value(tracee->proc, return_info))
			{
			  for (i = 0; i < nbrtab; i++)
			    pverbose(3, "\t|");
			  pverbose(3, " %s %s() = %s\n", return_info->type, return_info->name, return_info->value);
			}
		    }
		  return_debuginfo_list = list_del(return_debuginfo_list, return_info);
		}
	      /*
	      ** Print ret address
	      */
	      for (i = 0; i < nbrtab; i++)
		pverbose(1, "\t|");

#ifdef __USE_LONG_ADDR_T__
	      pverbose(1, " ret @ 0x%lx", addr);
#else
	      pverbose(1, " ret @ 0x%x", addr);
#endif /* __USE_LONG_ADDR_T__ */

	      pverbose(1, "\n");

	      nbrtab--;
	    }
	}
    }
}

int	tracing_loop(tracee_t* tracee, addr_t start)
{
  int		signal, ret, count = 0;
  addr_t	addr;


  pverbose(1, "\nStarting tracing !\n\n");
  if (!start)
    {
      if (proc_step(tracee->proc, 0) == -1)
	errexit("ftrace: proc_step, %s", ftrace_strerror(errno));
    }
  tracee->status = STEP;
  while (1)
    {
      ret = proc_wait_debug_event(tracee->proc, &signal);
      if (ret == -1)
	errexit("ftrace: proc_wait_debug_event, %s", ftrace_strerror(errno));
      switch (ret)
	{
	case DEBUG_EVENT:
	  count++;
	  if (tracer_quit)
	    return (0);
	  /*
	  ** Treat event
	  */
	  if (start)
	    {
	      addr = start;
	      ret = proc_seek_calls(tracee->proc, &addr);
	      if (ret == -1)
		errexit("ftrace: proc_seek_calls, %s", ftrace_strerror(errno));
	      print_call(tracee, ret, addr);
	      start = 0;
	    }
	  addr = 0;
	  ret = proc_seek_calls(tracee->proc, &addr);
	  if (ret == -1)
	    errexit("ftrace: proc_seek_calls, %s", ftrace_strerror(errno));
	  print_call(tracee, ret, addr);
	  if (proc_step(tracee->proc, 0) == -1)
	    errexit("ftrace: proc_step, %s", ftrace_strerror(errno));
	  break;
	case SIGNAL_EVENT:
	  pverbose(1, "Program received signal %d, %s\n", signal, sys_siglist[signal]);
	  proc_step(tracee->proc, signal);
	  break;
	case EXIT_EVENT:
	  pverbose(1, "\nProgram exited normally.\n");
	  return (0);
	case EXIT_SIGNALED_EVENT:
	  pverbose(1, "\nProgram exited with signal %d, %s\n", signal, sys_siglist[signal]);
	  return (0);
	default:
	  errexit("ftrace: proc_wait_debug_event, Unknow Error !");
	}
    }
  return (-1);
}


/* int	flush_tracing_loop(tracee_t* tracee) */
/* { */
/*   int		signal, ret; */

/*   printf("HELLO :o)"); */
/*   tracee->status = STEP; */
/*   while (1) */
/*     { */
/*       ret = proc_wait_debug_event_nohang(tracee->proc, &signal); */
/*       if (ret == -1) */
/* 	errexit("ftrace: proc_wait_debug_event, %s", ftrace_strerror(errno)); */
/*       switch (ret) */
/* 	{ */
/* 	case NOTHING_EVENT: */
/* 	  return (0); */
/* 	  break; */
/* 	case DEBUG_EVENT: */
/* 	  proc_continue(tracee->proc, 0); */
/* 	  break; */
/* 	case SIGNAL_EVENT: */
/* 	  printf("Program received signal %d, %s\n", signal, "todo" /\* sys_siglist[signal] *\/); */
/* 	  proc_step(tracee->proc, signal); */
/* 	  break; */
/* 	case EXIT_EVENT: */
/* 	  printf("\nProgram exited normally.\n"); */
/* 	  return (0); */
/* 	case EXIT_SIGNALED_EVENT: */
/* 	  printf("Program exited with signal %d, %s\n", signal, "todo" /\* sys_siglist[signal] *\/); */
/* 	  return (0); */
/* 	default: */
/* 	  errexit("ftrace: proc_wait_debug_event, Unknow Error !"); */
/* 	} */
/*     } */
/*   return (-1); */
/* } */

