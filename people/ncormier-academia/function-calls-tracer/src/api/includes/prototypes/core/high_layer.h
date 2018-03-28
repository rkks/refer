/*
** high_layer.h for  in /home/nico/lang/c/ftrace/src/api/includes/prototypes/core
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 17:38:04 2006 nicolas
** Last update Sat Apr  8 18:45:51 2006 nicolas
*/

#ifndef __CORE_HIGH_LAYER_H__
# define __CORE_HIGH_LAYER_H__

# include "api/includes/types/core.h"
# include "api/includes/types/bin.h"
# include "api/includes/types/depend.h"
# include "api/includes/types/prim_types.h"
# include "api/includes/types/regs.h"
# include "api/includes/types/backtrace.h"


regs_t*		core_get_registers(core_obj_t*);
char*		core_get_bin_path(core_obj_t*);
depends_t*	core_get_depends_list(core_obj_t*, bin_obj_t*);
backtrace_t*	core_backtrace(core_obj_t*);


#endif /* __BIN_HIGH_LAYER_H__ */
