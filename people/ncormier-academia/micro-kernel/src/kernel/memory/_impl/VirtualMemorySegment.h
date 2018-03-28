/*
** VirtualMemorySegment.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_VIRTUAL_MEMORY_SEGMENT_H__
# define __KERNEL_MEMORY__IMPL_VIRTUAL_MEMORY_SEGMENT_H__


# include <memory/IVirtualMemorySegment.h>


/* VirtualMemorySegment Object
 */
typedef struct _virtualMemorySegment
{
  void			(*init)					(struct _virtualMemorySegment* self, vaddr_t base_addr, offset_t size, 
								 IMemoryObject source, vms_access_t access, vms_owner_t owner,
								 vms_type_t type, IVirtualAddress addr_space);
  void			(*mapMemoryPageAtVirtualAddress)	(struct _virtualMemorySegment* self, IMemoryPage mem_page, 
								 vaddr_t vaddr);
  void			(*unmapVirtualMemoryPage)		(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page);
  void			(*remapVirtualMemoryPage)		(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page,
								 IMemoryPage mem_page);
  void			(*pageHasBeenSwappedIn)			(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page);
  void			(*syncWithBackingStore)			(struct _virtualMemorySegment* self);
  //handle_page_fault_res_t	(*handlePageFault)
  vaddr_t		(*baseAddress)				(struct _virtualMemorySegment* self);
  size_t		(*size)					(struct _virtualMemorySegment* self);
  vms_access_t		(*access)				(struct _virtualMemorySegment* self);
  vms_owner_t		(*owner)				(struct _virtualMemorySegment* self);
  vms_type_t		(*type)					(struct _virtualMemorySegment* self);
  IVirtualAddressSpace	(*vspace)				(struct _virtualMemorySegment* self);
  unsigned		(*mappedPagesCount)			(struct _virtualMemorySegment* self);
  IVirtualMemoryPage	(*mappedPageAtIndex)			(struct _virtualMemorySegment* self, unsigned index);
  IVirtualMemoryPage	(*mappedPageByAddress)			(struct _virtualMemorySegment* self, vadd_t addr);

  /* Private members
   */
  vaddr_t						_baseAddress;
  size_t						_size;
  vms_access_t						_access;
  vms_owner_t						_owner;
  vms_type_t						_type;
  IVirtualAddressSpace					_vspace;
  IMemoryObject						_source;
  SINGLY_LINKED_LIST(mappglst, IVirtualMemoryPage)	_mappedPageList;

} __attribute__ ((packed)) * VirtualMemorySegment;

/* Functions
 */
VirtualMemorySegment	alloc_memory_virtual_segment(void);


#endif /* __KERNEL_MEMORY__IMPL_VIRTUAL_MEMORY_SEGMENT_H__ */
