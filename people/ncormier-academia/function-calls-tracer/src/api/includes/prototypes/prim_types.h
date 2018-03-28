/*
** prim_types.h for  in /home/nico/lang/c/ftrace/src/api/includes/prototypes
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Thu Mar 23 17:23:47 2006 nicolas
** Last update Wed Apr  5 23:02:23 2006 nicolas
*/

#ifndef __PROTOTYPES_PRIM_TYPES_H__
# define __PROTOTYPES_PRIM_TYPES_H__

# include "api/includes/types/prim_types.h"

list_t*		list_add(list_t*, void*);
list_t*		list_del(list_t*, void*);
void		list_free(list_t*);

#endif /* __PROTOTYPES_PRIM_TYPES_H__ */
