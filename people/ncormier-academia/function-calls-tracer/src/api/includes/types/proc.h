/*
** proc.h for  in /home/nico/lang/c/ftrace/src/api/includes
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Wed Mar 22 16:52:25 2006 nicolas
** Last update Fri Apr 14 21:23:21 2006 nicolas cormier
*/

#ifndef __TYPES_PROC_H__
# define __TYPES_PROC_H__

# include "prim_types.h"

# include <sys/types.h>

struct proc_obj_s
{
  char*		path;
  pid_t		pid;
  int		attach;
  void*		luse;
};
typedef struct proc_obj_s  proc_obj_t;


struct brkp_s
{
  addr_t	addr;
  addr_t	sinc;
  int		set;
};
typedef struct brkp_s brkp_t;

typedef list_t  brkps_t;


#endif /* __TYPES_PROC_H__ */
