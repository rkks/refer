/*
** vm.c for  in /home/nico/lang/kernel/kld-netmalloc
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
#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_page.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <machine/memdev.h>

#include "vm.h"

vm_offset_t	vm_map_free_addr(vm_map_t mapa, vm_size_t size)
{
  size = round_page(size);
  vm_offset_t addr = vm_map_min(mapa);
  int result = vm_map_find(mapa, NULL, 0, &addr, size, TRUE, VM_PROT_ALL, VM_PROT_ALL, MAP_NOFAULT);
  if (result)
    return 0;
  return addr;
}

int	vm_map_kaddr_at_uaddr(vm_map_t mapa, vm_offset_t kaddr, vm_offset_t uaddr)
{
  vm_map_t map = mapa;
  vm_page_t kern_pg, user_pg;
  vm_object_t uobject;
  vm_map_entry_t entry;
  vm_pindex_t upindex;
  vm_prot_t prot;
  boolean_t wired;

  KASSERT((uaddr & PAGE_MASK) == 0, ("vm_map_kaddr_at_uaddr: uaddr is not page aligned"));
  kern_pg = PHYS_TO_VM_PAGE(vtophys(kaddr));
  kern_pg->valid = VM_PAGE_BITS_ALL;
  KASSERT(kern_pg->queue == PQ_NONE && kern_pg->wire_count == 1, ("vm_map_kaddr_at_uaddr: kern_pg is not correctly wired"));
  if (vm_map_lookup(&map, uaddr, VM_PROT_READ, &entry, &uobject, &upindex, &prot, &wired))
    return -1;
  VM_OBJECT_LOCK(uobject);
  user_pg = vm_page_lookup(uobject, upindex);
  if (user_pg != NULL)
    {
      /* User have already a page at this address
       */
      return -1;
    }
  if (uobject->backing_object != NULL)
    pmap_remove(map->pmap, uaddr, uaddr + PAGE_SIZE);
  vm_page_lock_queues();
#if 0
  vm_page_insert(kern_pg, uobject, upindex);
  vm_page_dirty(kern_pg);
#else
/*   vm_page_busy(kern_pg); */
  vm_page_rename(kern_pg, uobject, upindex);
/*   vm_page_flag_clear(kern_pg, PG_BUSY); */
/*   kern_pg->valid = VM_PAGE_BITS_ALL; */

  vm_page_dirty(kern_pg);

#endif
  vm_page_unlock_queues();
  VM_OBJECT_UNLOCK(uobject);
  vm_map_lookup_done(map, entry);
  return 0;
}
