/*
** arch_regs.h for  in /u/ept3/cormie_n/lang/c/ftrace/src/lib/solaris9_sparc
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Apr  5 19:45:14 2006 nicolas cormier
** Last update Wed Apr 26 11:55:03 2006 nicolas cormier
*/

#ifndef __ARCH_REGS_H__
# define __ARCH_REGS_H__

# ifndef _STRUCTURED_PROC
#  define _STRUCTURED_PROC	1
# endif

# include <sys/procfs.h>
# undef _STRUCTURED_PROC

struct regs_s
{
  prgregset_t	greg;
  int		has_gwin;
  gwindows_t	gwin;
};
typedef struct regs_s  regs_t;

#endif /* __ARCH_REGS_H__ */
