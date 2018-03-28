/*
** IVirtualAddressSpace.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_VIRTUAL_ADDRESS_SPACE_H__
# define __KERNEL_MEMORY_I_VIRTUAL_ADDRESS_SPACE_H__


# include <common/types.h>
# include <memory/IVirtualMemorySegment.h>
# include <memory/IMemoryPage.h>


/* IVirtualAddressSpace Object
 */
typedef struct _iVirtualAddressSpace
{
  void			(*init)			(struct _iVirtualAddressSpace* self);
  void			(*addSegment)		(struct _iVirtualAddressSpace* self, vaddr_t base_addr, offset_t size, 
						 IMemoryObject source, vms_access_t access, vms_owner_t owner, vms_type_t type);
  void			(*removeSegment)	(struct _iVirtualAddressSpace* self, IVirtualMemorySegment segment);
  unsigned		(*segmentsCount)	(struct _iVirtualAddressSpace* self);
  IVirtualMemorySegment	(*segmentAtIndex)	(struct _iVirtualAddressSpace* self, unsigned index);
  IVirtualMemorySegment	(*segmentByAddress)	(struct _iVirtualAddressSpace* self, vaddr_t vaddr);
  // handle_page_fault_res_t h handlePageFault()
  // HATL - private

} __attribute__ ((packed)) * IVirtualAddressSpace;


#endif /* __KERNEL_MEMORY_I_VIRTUAL_ADDRESS_SPACE_H__ */
