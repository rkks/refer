/*
** loop.h for  in /home/nico/lang/c/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Mon Apr  3 10:01:42 2006 nicolas
** Last update Wed Apr  5 23:07:50 2006 nicolas
*/

#ifndef __LOOP_H__
# define __LOOP_H__

# define TRACEE_OK	1
# define TRACEE_EXIT	2

# include "tracee.h"

int	init_loop(tracee_t*, addr_t*);
int	tracing_loop(tracee_t*, addr_t);

#endif /* __LOOP_H__ */
