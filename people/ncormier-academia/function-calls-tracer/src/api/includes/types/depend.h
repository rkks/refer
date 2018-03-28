/*
** depend.h for  in /home/nico/lang/c/ftrace/src/api/includes
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 16:48:46 2006 nicolas
** Last update Wed Apr  5 23:05:38 2006 nicolas
*/

#ifndef __TYPES_DEPEND_H__
# define __TYPES_DEPEND_H__

# include "prim_types.h"

struct depend_s
{
  char*		path;
  addr_t	base_addr;
};
typedef struct depend_s  depend_t;

typedef list_t  depends_t;

#endif /* __TYPES_DEPEND_H__ */
