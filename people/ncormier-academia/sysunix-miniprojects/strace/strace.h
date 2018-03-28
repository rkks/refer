/*
** strace.h for  in /home/nico/lang/c/strace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Wed Dec 28 18:19:46 2005 nicolas
** Last update Thu Jan  5 13:10:36 2006 nicolas cormier
*/

#ifndef __STRACE_H__
#define __STRACE_H__

#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>

void	errexit(char *format, ...);

#define USAGE "t3-strace [-h] [-v] {[-p pid] | [-e cmd]}"

typedef struct
{
  char	*type;
  void	(*func)(int);
}	pprint_func_t;

typedef struct
{
  int	val;
  char*	name;
}	flags_str_t;

typedef struct
{
  int	val;
  int	num;
}	flags_int_t;

#endif /* __STRACE_H__ */
