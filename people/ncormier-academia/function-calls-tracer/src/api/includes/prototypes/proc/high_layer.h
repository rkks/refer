/*
** high_layer.h for  in /home/nico/lang/c/ftrace/src/api/includes/prototypes/proc
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Wed Mar 22 17:49:55 2006 nicolas
** Last update Fri Apr 21 12:22:38 2006 nicolas
*/

#ifndef __PROC_HIGH_LAYER_H__
# define __PROC_HIGH_LAYER_H__

# include "api/includes/types/proc.h"
# include "api/includes/types/bin.h"
# include "api/includes/types/depend.h"
# include "api/includes/types/prim_types.h"
# include "api/includes/types/backtrace.h"

char*		proc_get_bin_path(proc_obj_t*);

depends_t*	proc_get_depends_list(proc_obj_t*);

backtrace_t*	proc_backtrace(proc_obj_t*);

int		proc_set_breakpoint(proc_obj_t*, brkp_t*);
int		proc_unset_breakpoint(proc_obj_t*, brkp_t*);

/*
** proc_seek_calls return
*/
# define CALL		1
# define RET		2
# define EXTCALL	3
# define PLTCALL	4
int		proc_seek_calls(proc_obj_t*, addr_t*);

int		proc_seek_args(proc_obj_t*, args_t*);

int		proc_seek_return_value(proc_obj_t*, arg_t*);

brkp_t*		proc_seek_brkps(proc_obj_t*, brkps_t*);

void		proc_dump_registers(proc_obj_t*);

int		proc_get_dependload_addr(proc_obj_t*, addr_t*);

#endif /* __PROC_HIGH_LAYER_H__ */
