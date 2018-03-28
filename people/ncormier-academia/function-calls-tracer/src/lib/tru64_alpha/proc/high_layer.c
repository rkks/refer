/*
** high_layer.c for  in /u/ept3/cormie_n/lang/c/ftrace/src/lib/tru64_alpha/proc
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Tue Apr 25 19:35:06 2006 nicolas cormier
** Last update Wed Apr 26 11:11:05 2006 nicolas cormier
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "api/includes/prototypes/proc/high_layer.h"
#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/prim_types.h"

#include "api/includes/types/prim_types.h"

char*	proc_get_bin_path(proc_obj_t* obj)
{
  obj = NULL;
  return (NULL);
}

list_t*	proc_get_depends_list(proc_obj_t* obj)
{
  obj = NULL;
  return (NULL);
}

list_t*	proc_backtrace(proc_obj_t* obj)
{
  obj = NULL;
  return (NULL);
}

int	proc_set_breakpoint(proc_obj_t* obj, brkp_t* brkp)
{
  obj = NULL;
  brkp = NULL;
  return (-1);
}

int	proc_unset_breakpoint(proc_obj_t* obj, brkp_t* brkp)
{
  obj = NULL;
  brkp = NULL;
  return (-1);
}

static addr_t	last_fp = 0;
int	proc_seek_calls(proc_obj_t* obj, addr_t* ioaddr)
{
  regs_t*	regs = NULL;
  int		ret = 0;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);
  *ioaddr = regs->r_pc;
  if (last_fp > (ulong) regs->r_fp)
    {
      last_fp = regs->r_fp;
      ret = CALL;
    }
  else if (last_fp < (ulong) regs->r_fp)
    {
      last_fp = regs->r_fp;
      ret = RET;
    }
  free(regs);
  return (ret);
}

brkp_t*	proc_seek_brkps(proc_obj_t* obj, brkps_t* brkps)
{
  obj = NULL;
  brkps = NULL;
  return (NULL);
}

void	proc_dump_registers(proc_obj_t* obj)
{
  obj = NULL;
}

int	proc_seek_args(proc_obj_t* obj, args_t* args)
{
  obj = NULL;
  args = NULL;
  return (-1);
}

int	proc_seek_return_value(proc_obj_t* obj, arg_t* ret)
{
  obj = NULL;
  ret = NULL;
  return (-1);
}

int	proc_get_dependload_addr(proc_obj_t* obj, addr_t* ret_addr)
{
  obj = NULL;
  ret_addr = NULL;
  return (-1);
}
