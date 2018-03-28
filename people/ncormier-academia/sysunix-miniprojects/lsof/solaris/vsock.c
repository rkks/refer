/*
** vsock.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 21:11:20 2005 nicolas cormier
** Last update Mon Nov 21 22:29:42 2005 nicolas cormier
*/

#include <stdio.h>

#define _KMEMUSER
#define _KERNEL

#include <kvm.h>
#include <sys/vnode.h>
#include <sys/socket.h>
#include <sys/systm.h>
#include <sys/socketvar.h>
#include <sys/sysmacros.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL

extern lsof_t line;

void		stat_sock(kvm_t* kd, vnode_t* vn)
{
  struct sonode	sn;

  KVM_READ_VRETURN(vn->v_data, &sn);

  sprintf(line.device, "-");

  if (!sn.so_protocol)
    sprintf(line.node, "TCP");
  else if (sn.so_protocol == 1)
    sprintf(line.node, "UDP");
  else
    sprintf(line.node, "%i", sn.so_protocol);

  sprintf(line.size, "-");


}
