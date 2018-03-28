/*
** lsof.h for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Nov 21 11:09:17 2005 nicolas cormier
** Last update Mon Nov 21 22:25:41 2005 nicolas cormier
*/

#ifndef __LSOF_H__
#define __LSOF_H__

#define	KVM_READ_CONTINUE(ptr, buf) \
			if (kvm_read(kd, (uintptr_t) ptr, buf, sizeof(*buf)) != sizeof(*buf)) \
			     continue;

#define	KVM_READ_VRETURN(ptr, buf) \
			if (kvm_read(kd, (uintptr_t) ptr, buf, sizeof(*buf)) != sizeof(*buf)) \
			     return;

#define	KVM_READ_IRETURN(ptr, buf) \
			if (kvm_read(kd, (uintptr_t) ptr, buf, sizeof(*buf)) != sizeof(*buf)) \
			     return (0);

#define	KVM_READ(ptr, buf) \
		kvm_read(kd, (uintptr_t) ptr, buf, sizeof(*buf));


typedef struct lsof
{
  char		cmd[256];
  int		pid;
  char		user[256];
  char		fd[256];
  char		type[256];
  char		device[256];
  char		size[256];
  char		node[256];
  char		name[256];
  char		fstype[256];
}lsof_t ;


void	errexit(char *format, ...);
void	ufs_stat(kvm_t* kd, vnode_t *f_vnode);
void	stat_proc(kvm_t* kd, proc_t* p_proc);
char*	get_node_type(kvm_t* kd, vnode_t* vn);
void    ufs_stat(kvm_t* kd, vnode_t *f_vnode);
void    nfs_stat(kvm_t* kd, vnode_t *f_vnode);
void    stat_dev(kvm_t* kd, vnode_t *f_vnode);
void    stat_fifo(kvm_t* kd, vnode_t *f_vnode);
void    stat_door(kvm_t* kd, vnode_t *f_vnode);
void    stat_sock(kvm_t* kd, vnode_t *f_vnode);

#endif /* __LSOF_H__ */
