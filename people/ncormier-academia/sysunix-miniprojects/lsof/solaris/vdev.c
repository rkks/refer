/*
** vdev.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 19:22:39 2005 nicolas cormier
** Last update Mon Nov 21 22:29:11 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/vnode.h>
#include <sys/fs/snode.h>
#include <sys/sysmacros.h>
#include <sys/ddi_impldefs.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t line;

void			stat_dev(kvm_t* kd, vnode_t* vn)
{
/*   struct snode		sn; */
/*   struct vnode		common; */
/*   struct dev_info	di; */
/*   char			buf[32]; */

  sprintf(line.device, "%d,%d", (int) getmajor(vn->v_rdev), (int) getminor(vn->v_rdev));
/*   KVM_READ_VRETURN(vn->v_data, &sn); */
/*   KVM_READ_VRETURN(sn.s_commonvp, &common); */
/*   KVM_READ_VRETURN(common.v_data, &sn); */

/*   if (sn.s_dip) */
/*     { */
/*       KVM_READ_VRETURN(sn.s_dip, &di); */
/*       KVM_READ_VRETURN(common.v_data, &buf); */
/*       sprintf(line.name, "[%x]", di->devi_name); */
/*       sprintf(line.size, "[%d]", di.devi_pm_comp_size); */
/*       sprintf(line.node, "[%i]", (int) di.devi_devid); */
/*     } */
  sprintf(line.size, "-");
  sprintf(line.node, "-");
}
