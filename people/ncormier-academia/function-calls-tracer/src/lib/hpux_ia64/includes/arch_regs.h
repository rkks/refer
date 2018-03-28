/*
** arch_regs.h for ftrace in /u/ept3/barbot_u/work/ftrace/src/_julien
** 
** Made by julieN barbot
** Login   <barbot_u@epita.fr>
** 
** Started on  Wed Apr 26 14:48:32 2006 julieN barbot
** Last update Wed Apr 26 15:09:31 2006 julieN barbot
*/

#ifndef __ARCH_REGS_H__
#define __ARCH_REGS_H__

#include <sys/types.h>
#include <machine/sys/uregs.h>

typedef struct	reg
{
  uint64_t	reason;	// Reason id (in case of an exception)
  uint64_t	ip;	// Instruction Pointer
  uint64_t	sp;	// Stack Pointer
  uint64_t	bsp;	// ???
  uint64_t	pfs;	// Previous Fonction State
}		regs_t;

#endif /* __ARCH_REGS_H__ */
