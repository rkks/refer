/*
** lsof.c for  in /u/ept3/cormie_n/lang/c/lsof
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Nov 16 14:10:12 2005 nicolas cormier
** Last update Mon Nov 21 22:24:21 2005 nicolas cormier
*/

#include <pwd.h>
#include <stdarg.h>
#include <unistd.h>
#include <strings.h>

#define _KMEMUSER
#define _KERNEL

#include <fcntl.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/vnode.h>
#include <sys/vfs.h>
#include <sys/sysmacros.h>
#include <vm/as.h>

#include "lsof.h"

#undef _KMEMUSER
#undef _KERNEL


lsof_t	line;

#define COLPRINT "%-15s%-10d%-10s%-10s%-10s%-15s%-10s%-15s%-10s%-10s\n"
#define COLARG 	 line.cmd, line.pid, line.user, line.fd, line.type, \
		 line.device, line.size, line.node, line.fstype, line.name

int		main(int argc, char **argv)
{
  kvm_t*	kd;
  proc_t*	p_proc;
  proc_t	p_link;

  if (!(kd = kvm_open(0x0, 0x0, 0x0, O_RDONLY, "lsof: ")))
      errexit("%s: kvm_open failed.", argv[0]);

  printf("%-15s%-10s%-10s%-10s%-10s%-15s%-10s%-15s%-10s%-10s\n",
	 "CMD", "PID", "USER", "FD", "TYPE", "DEVICE", "SIZEOFF", "NODE", "FSTYPE", "NAME");

  for (p_proc = kvm_nextproc(kd); p_proc; p_proc = kvm_nextproc(kd))
    {
      stat_proc(kd, p_proc);
      KVM_READ(p_proc->p_link, &p_link);
      stat_proc(kd, &p_link);
    }

  if (kvm_close(kd) == -1)
    errexit("%s: kvm_close failed.", argv[0]);;
  return (0);
}

void	stat_vnode(kvm_t* kd, vnode_t* vn)
{
  sprintf(line.device, "%d,%d", (int) getmajor(vn->v_rdev), (int) getminor(vn->v_rdev));
  sprintf(line.size, "-");
  sprintf(line.node, "-");
  sprintf(line.name, "-");
  sprintf(line.fstype, get_node_type(kd, vn));

  if (!strncmp("ufs", get_node_type(kd, vn), 3))
    ufs_stat(kd, vn);
  else if (!strncmp("nfs", get_node_type(kd, vn), 3))
    nfs_stat(kd, vn);

  switch (vn->v_type)
    {
    case VNON :
      sprintf(line.type, "%s", "VNON");
      break;
    case VREG :
      sprintf(line.type, "%s", "VREG");
      break;
    case VDIR :
      sprintf(line.type, "%s", "VDIR");
      break;
    case VBLK :
      sprintf(line.type, "%s", "VBLK");
      stat_dev(kd, vn);
      break;
    case VCHR :
      sprintf(line.type, "%s", "VCHR");
      stat_dev(kd, vn);
      break;
    case VLNK :
      sprintf(line.type, "%s", "VLNK");
      break;
    case VFIFO :
      sprintf(line.type, "%s", "VFIFO");
      stat_fifo(kd, vn);
      break;
    case VDOOR :
      sprintf(line.type, "%s", "VDOOR");
      stat_door(kd, vn);
      break;
    case VPROC :
      sprintf(line.type, "%s", "VPROC");
      break;
    case VSOCK :
      sprintf(line.type, "%s", "VSOCK");
      stat_sock(kd, vn);
      break;
    case VBAD :
      sprintf(line.type, "%s", "VBAD");
      break;
    }
}

void	stat_proc_info(kvm_t* kd, proc_t* p_proc)
{
  struct cred	p_cred;
  struct passwd	*user_pwd = 0x0;
  struct pid	pid_info;

  KVM_READ(p_proc->p_pidp, &pid_info);
  sprintf(line.cmd, "%s", p_proc->p_user.u_comm);
  KVM_READ_VRETURN(p_proc->p_cred, &p_cred);
  user_pwd = getpwuid(p_cred.cr_uid);
  sprintf(line.user, "%s", user_pwd->pw_name);
  line.pid = pid_info.pid_id;
}

/* void		stat_pas(kvm_t* kd, struct as* proc_as) */
/* { */
/*   int		i; */
/*   vnode_t	vn; */
/*   vnode_t**	vntab; */
/*   struct as	p_as; */

/*   KVM_READ_VRETURN(proc_as, &p_as); */
/*   vntab = malloc(p_as.a_sizedir * sizeof(*vntab)); */
/*   KVM_READ_VRETURN(p_as.a_objectdir, vntab); */
/*   for (i = 0; i < p_as.a_sizedir; i++) */
/*     { */
/*       KVM_READ_CONTINUE(vntab[i], &vn); */
/*       stat_vnode(kd, &vn); */
/*       sprintf(line.fd, "txt"); */
/*       printf("%s\t%d\t%s\t%s\t%s\t%s\n", line.cmd, line.pid, line.user, line.fd, line.type, line.device); */
/*     } */
/* } */

void	stat_proc(kvm_t* kd, proc_t* p_proc)
{
  int		i;
  uf_entry_t*	cur_fi_ptr = 0x0;
  uf_entry_t	fi;
  file_t	uf_file;
  vnode_t	vn;

  stat_proc_info(kd, p_proc);
  for (i = 0; i < p_proc->p_user.u_finfo.fi_nfiles; i++)
    {
      cur_fi_ptr = (uf_entry_t*) p_proc->p_user.u_finfo.fi_list;
      cur_fi_ptr += i;

      KVM_READ_CONTINUE(cur_fi_ptr, &fi);
      KVM_READ_CONTINUE(fi.uf_file, &uf_file);
      KVM_READ_CONTINUE(uf_file.f_vnode, &vn);
      stat_vnode(kd, &vn);
      sprintf(line.fd, "%d", i);
      if ((uf_file.f_flag = (uf_file.f_flag & (FREAD | FWRITE))) == FREAD)
	sprintf(line.fd, "%sr", line.fd);
      else if (uf_file.f_flag == FWRITE)
	sprintf(line.fd, "%sw", line.fd);
      else if (uf_file.f_flag == (FREAD | FWRITE))
	sprintf(line.fd, "%su", line.fd);
      printf(COLPRINT, COLARG);
    }

/*   stat_pas(kd, p_proc->p_as); */
  KVM_READ_VRETURN(p_proc->p_user.u_cdir, &vn);
  stat_vnode(kd, &vn);
  sprintf(line.fd, "cwd");
  printf(COLPRINT, COLARG);
  KVM_READ_VRETURN(p_proc->p_user.u_rdir, &vn);
  stat_vnode(kd, &vn);
  sprintf(line.fd, "rtd");
  printf(COLPRINT, COLARG);
}

char*		get_node_type(kvm_t* kd, vnode_t* vn)
{
  vfs_t		v_vfsp;
  struct vfssw*	vswp;

  KVM_READ_IRETURN(vn->v_vfsp, &v_vfsp);
  vswp = &vfssw[v_vfsp.vfs_fstype];
  return (vswp->vsw_name);
}

void		errexit(char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s\n", buf);
  va_end(ap);
  _exit(1);
}
