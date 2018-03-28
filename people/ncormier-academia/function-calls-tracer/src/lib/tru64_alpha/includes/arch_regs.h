/*
** arch_regs.h for  in /u/ept3/cormie_n/lang/c/ftrace/src/lib/solaris9_sparc
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Apr  5 19:43:55 2006 nicolas cormier
** Last update Wed Apr 26 10:26:48 2006 nicolas cormier
*/

#ifndef __ARCH_REGS_H__
# define __ARCH_REGS_H__

#define _STRUCTURED_PROC	1
#include <sys/procfs.h>
#undef _STRUCTURED_PROC

struct regs_s
{
  long	r_pc;
  long	r_sp;
  long	r_ra;
  long	r_rv;
  long	r_fp;
};
typedef struct regs_s  regs_t;


#endif /* __ARCH_REGS_H__ */
