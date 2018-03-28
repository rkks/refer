/*
** netmalloc.c for  in /home/nico/lang/kernel/kld-netmalloc
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

#include <vm/vm.h>
#include <vm/pmap.h>

#include <machine/memdev.h>

#include "netmalloc_common.h"
#include "kernel-dependant.h"

MALLOC_DEFINE(M_NETMALLOC, "netmalloc-allocation", "netmalloc allocation");

static int	netmalloc_mmap(struct cdev *dev, vm_offset_t offset, vm_paddr_t* paddr, int prot __unused);

static struct cdev* netmalloc_dev;
static struct cdevsw netmalloc_cdevsw =
{
  .d_version =    D_VERSION,
  .d_mmap =       netmalloc_mmap,
  .d_name =       "netmalloc",
};

static int	netmalloc_mmap(struct cdev *dev, vm_offset_t offset, vm_paddr_t* paddr, int prot)
{
/*   void* kaddr = NULL; */
/*   int ret = netmalloc_action(curthread, netmalloc_mmap_passed_action(), &kaddr); */
/*   if (!ret && kaddr) /\* netmalloc_action succeed and something has been allocated *\/ */
/*     { */
/*       printf("~~~> %x\n", (unsigned)kaddr); */
      *paddr = vtophys(malloc(1024));
      return 0;
/*     } */
  return EOPNOTSUPP;
}

static int	netmalloc_modevent(module_t mod __unused, int type, void* data __unused)
{
  switch(type) 
    {
    case MOD_LOAD:
      netmalloc_dev = make_dev(&netmalloc_cdevsw, 0, UID_ROOT, GID_WHEEL, 0644, "netmalloc");
      break;
    case MOD_UNLOAD:
      destroy_dev(netmalloc_dev);
      break;
    case MOD_SHUTDOWN:
      break;
    default:
      return EOPNOTSUPP;
    }
  return 0;
}

DEV_MODULE(netmalloc, netmalloc_modevent, NULL);
