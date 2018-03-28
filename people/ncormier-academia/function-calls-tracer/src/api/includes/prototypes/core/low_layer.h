/*
** low_layer.h for  in /home/nico/lang/c/ftrace/src/api/includes/prototypes/core
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 17:29:33 2006 nicolas
** Last update Fri Apr  7 11:20:00 2006 nicolas
*/

#ifndef __CORE_LOW_LAYER_H__
# define __CORE_LOW_LAYER_H__

# include "api/includes/types/core.h"
# include "api/includes/types/prim_types.h"

core_obj_t*	core_new(char*);
int		core_del(core_obj_t*);

int		core_read(core_obj_t*, addr_t, unsigned int, char*);

#endif /* __CORE_LOW_LAYER_H__ */
