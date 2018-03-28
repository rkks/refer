/*
** brkps.c for  in /home/nico/lang/c/ftrace/src
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Sun Apr  2 17:24:39 2006 nicolas
** Last update Wed Apr 26 23:17:34 2006 nicolas
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "brkps.h"
#include "error.h"
#include "warning.h"
#include "verbose.h"

#include "api/includes/prototypes/proc/high_layer.h"
#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/bin/high_layer/bin.h"
#include "api/includes/prototypes/prim_types.h"


static addr_t	pow16(int pow)
{
  addr_t	ret;

  for (ret = 1; pow; pow--)
    ret *= 16;
  return (ret);
}

static addr_t	axtoi(const char* hex_str)
{
  addr_t	ret = 0, pow = 0;
  char*		end, * tmp;

  if (!strncmp(hex_str, "0x", 2))
    hex_str += 2;
  end = (char*) hex_str;
  for (tmp = (char*) hex_str; *tmp; tmp++)
    end = tmp;
  for (tmp = end; tmp >= hex_str; tmp--, pow++)
    {
      if (*tmp <= 'f' && *tmp >= 'a')
	ret += (*tmp - 'a' + 10) * pow16(pow);
      else if (*tmp >= '0' && *tmp <= '9')
	ret += (*tmp - '0') * pow16(pow);
      else
	{
	  errno = EINVAL;
	  break;
	}
    }
  return (ret);
}

static brkp_t*	get_brkp(brkps_t* brkps, addr_t addr)
{
  list_t*	cur;
  brkp_t*	brkp;

  for (cur = brkps; cur; cur = cur->next)
    {
      brkp = cur->value;
      if (brkp->addr == addr)
	return (brkp);
      if (cur == brkps->prev)
	break;
    }
  return (NULL);
}

/* void	fbrkps_reinject(tracee_t* tracee) */
/* { */
/*   list_t*	cur; */
/*   brkp_t*	brkp; */

/*   for (cur = tracee->fbrkps; cur; cur = cur->next) */
/*     { */
/*       brkp = cur->value; */

/*       if (brkp->set == 0) */
/* 	{ */
/* 	  if (proc_set_hardware_breakpoint(tracee->proc, brkp, DB_EXEC) == -1) */
/* 	    errexit("TODO"); */
/* 	} */

/*       if (cur == tracee->fbrkps->prev) */
/* 	break; */
/*     } */
/* } */

void	brkps_try_resolved(tracee_t* tracee)
{
  list_t*	cur;
  list_t*	todel = NULL;
  addr_t	addr;
  brkp_t*	brkp;

  /*
  ** Browse ARGV
  */
  for (cur = tracee->brkps_toset; cur; cur = cur->next)
    {
      errno = 0;
      /*
      ** if *ARGV is an hexadecimal address
      */
      addr = axtoi((const char*) cur->value);
      if (errno)
	{
	  /*
	  ** else treat *ARGV as a symbol.
	  */
	  if (!bin_find_addr_frm_label(tracee->binh, cur->value, &addr))
	    goto brkps_try_resolved_next;
	}
      /*
      ** Breakpoint addr found !
      ** Check if we haven't already add a breakpoint
      ** on this addr.
      */
      brkp = get_brkp(tracee->brkps, addr);
      if (!brkp)
	brkp = get_brkp(tracee->fbrkps, addr);
      if (!brkp)
	{
	  /*
	  ** Add breakpoint.
	  */
	  brkp = malloc(sizeof(brkp_t));
	  if (brkp == NULL)
	    errexit("ftrace: malloc:, %s", ftrace_strerror(errno));
	  memset(brkp, 0, sizeof(brkp_t));
	  brkp->addr = addr;
	  tracee->brkps = list_add(tracee->brkps, brkp);
	  if (tracee->brkps == NULL)
	    errexit("ftrace: list_add:, %s", ftrace_strerror(errno));
	}
      /*
      ** Breakpoint treated so we can delete it from the ARGV list.
      */
      todel = list_add(todel, cur->value);
      if (todel == NULL)
	errexit("ftrace: list_add:, %s", ftrace_strerror(errno));

    brkps_try_resolved_next:
      if (cur == tracee->brkps_toset->prev)
	break;
    }
  /*
  ** Set breakpoints on tracee.
  */
  for (cur = tracee->brkps; cur; cur = cur->next)
    {
      brkp = cur->value;
      if (brkp->set == 0)
	{
	  pverbose(4, "Set breakpoint at: 0x%x\n", brkp->addr);
	  if (proc_set_breakpoint(tracee->proc, brkp) == -1)
	    errexit("ftrace: proc_set_breakpoint:, %s", ftrace_strerror(errno));
	}
      if (cur == tracee->brkps->prev)
	break;
    }
  /*
  ** Delete from argv list each breakpoint setted.
  */
  for (cur = todel; cur; cur = cur->next)
    {
      tracee->brkps_toset = list_del(tracee->brkps_toset, cur->value);
      if (cur == todel->prev)
	break;
    }
}

void	brkps_inject(tracee_t* tracee)
{
  addr_t	addr;
  brkp_t*	brkp;

  /*
  ** Try to resolve & inject user breakpoints.
  */
  brkps_try_resolved(tracee);
  /*
  ** Inject ftrace's breakpoint,
  ** ftrace need a breakpoint to know when the linker
  ** have finished it work and when we can solve
  ** unsolved breakpoint or symbol.
  */
  if (proc_get_dependload_addr(tracee->proc, &addr) != -1 ||
      (tracee->binh->tracee &&
       bin_get_dependload_addr(tracee->binh->tracee->obj, &addr) != -1))
    {
	if (!get_brkp(tracee->brkps, addr))
	  {
	    brkp = malloc(sizeof(brkp));
	    if (brkp == NULL)
	      errexit("ftrace: malloc:, %s", ftrace_strerror(errno));
	    memset(brkp, 0, sizeof(brkp));
	    brkp->addr = addr;
	    /* if (proc_set_hardware_breakpoint(tracee->proc, brkp, DB_EXEC) == -1) */
	    /*   errexit("TODO"); */
	    if (proc_set_breakpoint(tracee->proc, brkp) == -1)
	      errexit("ftrace: proc_set_breakpoint:, %s", ftrace_strerror(errno));
	    tracee->fbrkps = list_add(tracee->fbrkps, brkp);
	    pverbose(4, "Set FTrace breakpoint at: 0x%x\n", brkp->addr);
	  }
    }
}

