/*
** I64_highproc.c for ftrace in /u/ept3/barbot_u/work/ftrace/src/_julien
** 
** Made by julieN barbot
** Login   <barbot_u@epita.fr>
** 
** Started on  Sat Apr 22 17:32:51 2006 julieN barbot
** Last update Wed Apr 26 16:06:02 2006 julieN barbot
*/

#include <stdlib.h>

#include "arch_regs.h"
#include "api/includes/prototypes/proc/high_layer.h"


char*	proc_get_bin_path(proc_obj_t* proc)
{
  proc = NULL;
  return (NULL);
}

depends_t*	proc_get_depends_list(proc_obj_t* proc)
{
  proc = NULL;
  return (NULL);
}

backtrace_t*	proc_backtrace(proc_obj_t* proc)
{
  proc = NULL;
  return (NULL);
}

int		proc_set_breakpoint(proc_obj_t* proc, brkp_t* brkp)
{
  proc = NULL;
  brkp = NULL;
  return (-1);
}

int		proc_unset_breakpoint(proc_obj_t* proc, brkp_t* brkp)
{
  proc = NULL;
  brkp = NULL;
  return (-1);
}



static addr_t	last_pfs = 0;
int		proc_seek_calls(proc_obj_t* obj, addr_t* ioaddr)
{
  regs_t*	regs = NULL;
  int		ret = 0;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);

/*   *ioaddr = regs->r_eip; */
  if (last_pfs > regs->pfs)
    {
      last_pfs = regs->pfs;
      ret = CALL;
      printf("Call --> [%x]\n", regs->pfs);
    }
  else if (last_pfs < regs->pfs)
    {
      last_pfs = regs->pfs;
      ret = RET;
    }
/*   if (addr_is_in_rtld(obj, regs->r_eip)) */
/*     ret = 0; */
  free(regs);
  return (ret);
}



int	proc_seek_args(proc_obj_t* proc, args_t* args)
{
  proc = NULL;
  args = NULL;
  return (-1);
}

int		proc_seek_return_value(proc_obj_t* proc, arg_t* args)
{
  proc = NULL;
  args = NULL;
  return (-1);
}

brkp_t*	proc_seek_brkps(proc_obj_t* proc, brkps_t* brkp)
{
  proc = NULL;
  brkp = NULL;
  return (-1);
}

void		proc_dump_registers(proc_obj_t* proc)
{
}

int		proc_get_dependload_addr(proc_obj_t* proc, addr_t* addr)
{
  proc = NULL; addr = NULL;
  return (-1);
}
