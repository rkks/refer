/*
** vdoor.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 15:23:06 2005 nicolas cormier
** Last update Mon Nov 21 22:30:49 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/door.h>
#include <sys/sysmacros.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t	line;

void			stat_door(kvm_t* kd, vnode_t* vn)
{
  struct door_node	dn;
  struct proc		door_target;
  struct pid		pid_info;

  sprintf(line.size, "0");
  KVM_READ_VRETURN(vn->v_data, &dn);
  sprintf(line.device, "%d,%d", (int) getmajor(dn.door_index), (int) getminor(dn.door_index));
  if (getminor(dn.door_index))
    sprintf(line.node, "%d", (int) getminor(dn.door_index));
  else
    sprintf(line.node, " ");
  KVM_READ_VRETURN(dn.door_target, &door_target);
  KVM_READ(door_target.p_pidp, &pid_info);
  if (line.pid == pid_info.pid_id)
    sprintf(line.name, "(this PID's door)");
  else
    sprintf(line.name, "(door to %d)", (int) pid_info.pid_id);
}
