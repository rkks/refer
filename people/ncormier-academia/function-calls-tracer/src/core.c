/*
** core.c for  in /home/nico/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Fri Apr  7 15:23:23 2006 nicolas
** Last update Thu Apr 27 13:08:29 2006 nicolas
*/

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "api/includes/prototypes/core/high_layer.h"
#include "api/includes/prototypes/core/low_layer.h"
#include "api/includes/prototypes/prim_types.h"
#include "api/includes/prototypes/bin/high_layer/symbols.h"
#include "api/includes/prototypes/bin/high_layer/debug.h"

#include "bin.h"
#include "core.h"
#include "error.h"
#include "verbose.h"

static int	nbrtab = 0;
static void	print_backt(bin_handler_t* binh, addr_t addr)
{
  bin_t*	bin;
  int		i;
  sym_t*	sym;
  debug_t*	debug;

  nbrtab++;
  for (i = 0; i < nbrtab; i++)
    pverbose(1, "\t");
  pverbose(1, " 0x%x", addr);
  
  bin = bin_find(binh, addr);

  if (bin)
    {
      sym = sym_get_frm_addr(bin->syms, addr, APPROACHED);
      if (sym)
	{
	  pverbose(2, " (%s)", sym->name);
	  debug = debug_get_frm_addr(bin->debugs, sym->addr);
	  if (debug)
	    pverbose(2, " @ %s:%d", debug->file, debug->line);
	}
      else
	pverbose(2, " (??)");
    }
  pverbose(1, "\n");
}

void	core_trace(char* path, char* binpath)
{
  core_obj_t*	core = NULL;
  bin_handler_t*	binh = NULL;
  backtrace_t*	backtrace = NULL;
  backt_t*	backt = NULL;

  core = core_new(path);
  if (core == NULL)
    errexit("ftrace: core_new:, %s", ftrace_strerror(errno));
  /*
  ** Init binaries.
  */
  if (binpath)
    binh = bin_create_handler(binpath);
  else
    binh = bin_create_handler(core_get_bin_path(core));
  if (binh == NULL)
    errexit("ftrace: bin_create_handler, %s", ftrace_strerror(errno));
  if (binh->tracee)
    bin_refresh_depends_list(core_get_depends_list(core, binh->tracee->obj), binh);
  backtrace = core_backtrace(core);
  if (backtrace == NULL)
    errexit("ftrace: core_backtrace, %s", ftrace_strerror(errno));
  /*
  ** Start backtrace.
  */
  pverbose(1, "\nStarting backtrace of %s\n\n", path);
  for (; backtrace; )
    {
      backt = (backt_t*) backtrace->value;

      print_backt(binh, backt->addr);

      backtrace = list_del(backtrace, backt);
      free(backt);
    }
  pverbose(1, "\n");
}
