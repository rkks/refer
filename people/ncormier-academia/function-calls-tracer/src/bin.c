/*
** bin.c for  in /home/nico/lang/c/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 29 19:22:05 2006 nicolas
** Last update Thu Apr 27 13:07:34 2006 nicolas
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bin.h"
#include "error.h"
#include "warning.h"
#include "verbose.h"

#include "api/includes/types/depend.h"

#include "api/includes/prototypes/prim_types.h"
#include "api/includes/prototypes/bin/low_layer.h"
#include "api/includes/prototypes/proc/high_layer.h"
#include "api/includes/prototypes/bin/high_layer/bin.h"
#include "api/includes/prototypes/bin/high_layer/symbols.h"
#include "api/includes/prototypes/bin/high_layer/debug.h"

static bin_t*	fill_bin(char* path, addr_t base)
{
  bin_obj_t*	binobj = NULL;
  syms_t*	syms = NULL;
  bin_t*	bin = NULL;
  debugs_t*	debugs = NULL;

  /*
  ** Open obj's elf binary file.
  */
  binobj = bin_new(path, base);
  if (binobj == NULL)
    goto fill_bin_failed;
  /*
  ** Create symbols list from binary.
  */
  syms = syms_new(binobj);
  /*
  ** Create debug info list from binary.
  */
  debugs = debug_new_from_bin(binobj);
  /*
  ** fill obj's bin
  */
  bin = malloc(sizeof(bin_t));
  if (bin == NULL)
    goto fill_bin_failed;
  bin->obj = binobj;
  bin->syms = syms;
  bin->debugs = debugs;
  pverbose(4, "New shared object: %s @ 0x%x\n", path, base);
  return (bin);
  /*
  ** If something's gone wrong.
  */
 fill_bin_failed:
  if (binobj)
    free(binobj);
  if (bin)
    free(bin);
  if (syms)
    free(syms);
  if (debugs)
    free(debugs);
  return (NULL);
}

int	bin_refresh_depends_list(depends_t* depends, bin_handler_t* binh)
{
  list_t*	list = NULL;
  list_t*	listb = NULL;
  depend_t*	dep = NULL;
  int		found = 0, newdep = 0;
  bin_t*	bin = NULL;

  /*
  ** Browse current depends list.
  */
  for (list = (list_t*) depends; list; list = list->next)
    {
      dep = (depend_t*) list->value;
      /*
      ** Check if we already loaded the depend.
      */
      for (listb = binh->depends, found = 0; listb; listb = listb->next)
	{
	  if (!strcmp(
		      ((bin_t*)listb->value)->obj->path ,
		      dep->path)
	      )
	    {
	      found = 1;
	      break;
	    }

	  if (listb == binh->depends->prev)
	    break;
	}
      if (!found)
	{
	  /*
	  ** Add new depend's binary to binary depends list.
	  */
	  bin = fill_bin(dep->path, dep->base_addr);
	  if (bin != NULL)
	    {
	      binh->depends = list_add((list_t*) binh->depends, bin);
	      if (binh->depends == NULL)
		errexit("ftrace: list_add:, %s", ftrace_strerror(errno));
	      newdep = 1;
	    }
	}

      if (list == ((list_t*) depends)->prev)
	break;
    }
  /*
  ** Free depends.
  */
  for (; depends; )
    {
      dep = (depend_t*) depends->value;
      depends = list_del(depends, dep);
      if (dep && dep->path)
	free(dep->path);
      free(dep);
    }  
  return (newdep);
}

bin_handler_t*	bin_create_handler(char* path)
{
  bin_handler_t*	ret = NULL;
  bin_t*	bin = NULL;

  /* 
  ** Create tracee's bin
  */
  if (path)
    bin = fill_bin(path, 0);
  if (bin == NULL)
    {
      if (path)
	pwarn("Warning: can't load local symbol ! (%s)\n", path);
      else
	pwarn("Warning: can't load local symbol ! (binary not found)\n");
    }
  ret = malloc(sizeof(bin_handler_t));
  if (ret == NULL)
    return (NULL);
  ret->tracee = bin;
  ret->depends = NULL;
  return (ret);
}

bin_t*	bin_find(bin_handler_t* binh, addr_t addr)
{
  bin_t*	cur = NULL;
  list_t*	list = NULL;
  
  if (binh->tracee && bin_contain(binh->tracee->obj, addr))
    return (binh->tracee);

  for (list = (list_t*) binh->depends; list; list = list->next)
    {
      cur = (bin_t*) list->value;

      if (bin_contain(cur->obj, addr))
	return (cur);

      if (list == ((list_t*) binh->depends)->prev)
	break;
    }
  return (NULL);
}

int	bin_find_addr_frm_label(bin_handler_t* binh, char* label, addr_t* retaddr)
{
  bin_t*	cur = NULL;
  sym_t*	sym = NULL;
  list_t*	list = NULL;
  
  if (binh->tracee)
    {
      sym = sym_get_frm_label(binh->tracee->syms, label);
      if (sym)
	{
	  *retaddr = sym->addr;
	  return (1);
	}
    }


  for (list = (list_t*) binh->depends; list; list = list->next)
    {
      cur = (bin_t*) list->value;

      sym = sym_get_frm_label(cur->syms, label);
      if (sym)
	{
	  *retaddr = sym->addr;
	  return (1);
	}

      if (list == ((list_t*) binh->depends)->prev)
	break;
    }
  return (0);
}
