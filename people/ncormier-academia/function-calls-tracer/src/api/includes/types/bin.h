/*
** bin.h for  in /home/nico/lang/c/ftrace/src/api/includes
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 16:42:52 2006 nicolas
** Last update Tue Apr 11 15:22:52 2006 nicolas
*/

#ifndef __TYPES_BIN_H__
# define __TYPES_BIN_H__

# include "prim_types.h"

struct bin_obj_s 
{
  char*		path;
  void*		map;
  int		fd;
  unsigned int	size;
  void*		luse;
};
typedef struct bin_obj_s  bin_obj_t;


struct sym_s
{
  char*		name;
  addr_t	addr;
  unsigned int	size;
};
typedef struct sym_s  sym_t;

typedef list_t  syms_t;


struct arg_s
{
  unsigned int	order;
  char*		type;
  char*		name;
  unsigned int	size;
  char		value[256];
}; 
typedef struct arg_s  arg_t;

typedef list_t args_t;


struct debug_s
{
  addr_t	addr;
  char*		name;
  char*		file;
  unsigned int	line;
  arg_t*	ret;
  args_t*	args;
};
typedef struct debug_s  debug_t;

typedef list_t	debugs_t;

#endif /* __TYPES_BIN_H__ */
