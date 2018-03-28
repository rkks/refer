/*
** backtrace.h for  in /home/nico/lang/c/ftrace/src/api/includes
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 16:53:20 2006 nicolas
** Last update Wed Apr  5 23:04:37 2006 nicolas
*/

#ifndef __TYPES_BACKTRACE_H__
# define __TYPES_BACKTRACE_H__

# include "prim_types.h"

struct backt_s
{
  addr_t	addr;
  unsigned int	count;
};
typedef struct backt_s  backt_t;

typedef list_t	backtrace_t;

#endif /* __TYPES_BACKTRACE_H__ */
