/*
** ufs.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 11:36:28 2005 nicolas cormier
** Last update Mon Nov 21 22:28:47 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/vnode.h>
#include <sys/fs/ufs_inode.h>
#include <sys/sysmacros.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t line;

void	ufs_stat(kvm_t* kd, vnode_t *f_vnode)
{
  struct inode inode;
  int	nb_read;

  nb_read = kvm_read(kd, (uintptr_t) VTOI(f_vnode), &inode, sizeof(inode));
  if (nb_read != sizeof(inode))
    return;
  sprintf(line.device, "%d,%d", (int) getmajor(inode.i_dev), (int) getminor(inode.i_dev));
  sprintf(line.size, "%d", (int) inode.i_size);
  sprintf(line.node, "%i", (int) inode.i_number);
}
