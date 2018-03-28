/*
** vm.h for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#ifndef __VM_H__
# define __VM_H__

# include <vm/vm.h>
# include <vm/pmap.h>

vm_offset_t	vm_map_free_addr(vm_map_t mapa, vm_size_t size);
int		vm_map_kaddr_at_uaddr(vm_map_t mapa, vm_offset_t kaddr, vm_offset_t uaddr);

#endif /* __VM_H__ */
