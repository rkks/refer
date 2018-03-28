/*
** netmalloc.sys.c for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/fcntl.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/signalvar.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <sys/cdefs.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/sysent.h>
#include <sys/module.h>
#include <sys/kthread.h>
#include <sys/resource.h>
#include <sys/resourcevar.h>
#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>

#include "netmalloc_common.h"
#include "vm.h"
#include "die.h"
#include "server.h"

/* syscall module
 */

struct netmalloc_sysc_args
{
  struct netmalloc_args*	args;
};

static int	netmalloc_sysc(struct thread* th, struct netmalloc_sysc_args *user_args)
{
  void* kaddr = NULL;
  int ret = netmalloc_action(th, user_args->args, &kaddr);
  if (!ret && kaddr) /* netmalloc_action succeed and something has been allocated */
    {
#if 0
      /* Map in userspace
       */
      vm_offset_t uaddr = vm_map_free_addr(&th->td_proc->p_vmspace->vm_map, user_args->args->size);
      if (vm_map_kaddr_at_uaddr(&th->td_proc->p_vmspace->vm_map, (vm_offset_t)kaddr, uaddr))
	die("netmalloc_sysc: vm_map_kaddr_at_uaddr failed");
      *user_args->args->o_vaddr = (void*)uaddr;
#else
      *user_args->args->o_vaddr = (void*)kaddr;

#endif
    }
  return ret;
}

static struct sysent netmalloc_sysc_sysent = 
{
  2, /* Number of parameters */ 
  (sy_call_t *)netmalloc_sysc	/* syscall func */ 
};

static int syscall_num = NO_SYSCALL;

static int	module_handler(struct module *m, int what, void *arg)
{
  int err = 0;
  switch (what) {
  case MOD_LOAD:
    uprintf("Client system call loaded at slot: %d\n", syscall_num);
    break;
  case MOD_UNLOAD:
    break;
  default:
    err = EINVAL;
    break;
  }
  return(err);
}

SYSCALL_MODULE(netmalloc_sysc,  /* name */
	       &syscall_num, /* syscall number */
	       &netmalloc_sysc_sysent, /* sysent struct */
               module_handler, /* module handler func */
	       NULL); /* module handler arg */
