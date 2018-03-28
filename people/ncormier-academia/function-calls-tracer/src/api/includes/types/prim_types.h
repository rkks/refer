/*
** prim_types.h for  in /home/nico/lang/c/ftrace/src/api/includes/types
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Wed Mar 22 16:57:02 2006 nicolas
** Last update Tue Apr 25 20:47:54 2006 nicolas
*/

#ifndef __TYPES_PRIM_TYPES_H__
# define __TYPES_PRIM_TYPES_H__


# define addr_t ftrace_addr_t

# ifdef __USE_LONG_ADDR_T__
	typedef unsigned long  ftrace_addr_t;
# else
	typedef unsigned int    ftrace_addr_t;
# endif /* __USE_LONG_ADDR_T__ */



struct list_s
{
  struct list_s*        next;
  struct list_s*        prev;
  void*                 value;
};
typedef struct list_s  list_t;


#endif /* __TYPES_PRIM_TYPES_H__ */
