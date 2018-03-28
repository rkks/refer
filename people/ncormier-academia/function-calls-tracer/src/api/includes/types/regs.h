/*
** regs.h for  in /home/nico/lang/c/ftrace/src/api/includes/types
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Wed Mar 22 17:34:37 2006 nicolas
** Last update Wed Apr  5 23:06:57 2006 nicolas
*/

#ifndef __TYPES_REGS_H__
# define __TYPES_REGS_H__


# ifdef __FTRACE__

typedef void  regs_t;

# else

#  include "arch_regs.h"

# endif


#endif /* __TYPES_REGS_H__ */
