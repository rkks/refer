/*
** vfifo.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 10:50:18 2005 nicolas cormier
** Last update Mon Nov 21 22:29:35 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/vnode.h>
#include <sys/fs/fifonode.h>
#include <sys/sysmacros.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t line;

void			stat_fifo(kvm_t* kd, vnode_t* vn)
{
  struct fifonode	fn;
  struct vnode		rvn;

  KVM_READ_VRETURN(vn->v_data, &fn);
  sprintf(line.node, "%i", (int) fn.fn_ino);
  KVM_READ_VRETURN(fn.fn_realvp, &rvn);
/*   sprintf(line.device, "[%d,%d]", getmajor(rvn.v_rdev), getminor(rvn.v_rdev)); */
/*   sprintf(line.device, "[%d,%d]", getmajor(fn.fn_vnode.v_rdev), getminor(fn.fn_vnode.v_rdev)); */
/*   sprintf(line.size, "%d", fn.fn_count); */
  sprintf(line.node, "-");
  sprintf(line.device, "-");
  sprintf(line.size, "-");
}
