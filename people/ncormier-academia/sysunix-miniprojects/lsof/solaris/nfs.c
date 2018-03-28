/*
** nfs.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 11:36:28 2005 nicolas cormier
** Last update Mon Nov 21 22:28:07 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/vnode.h>
#include <nfs/nfs.h>
#include <nfs/rnode.h>
#include <sys/sysmacros.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t line;

void		nfs_stat(kvm_t* kd, vnode_t *vn)
{
  struct rnode	rn;

  KVM_READ_VRETURN(VTOR(vn), &rn);
  sprintf(line.device, "%d,%d", (int) getmajor(rn.r_attr.va_fsid), (int) getminor(rn.r_attr.va_fsid));
  sprintf(line.size, "%d", (int) rn.r_attr.va_size);
  sprintf(line.node, "%i", (int) rn.r_attr.va_nodeid);
}
