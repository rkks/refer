/*
** IVirtualMemorySegment.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_VIRTUAL_MEMORY_SEGMENT_H__
# define __KERNEL_MEMORY_I_VIRTUAL_MEMORY_SEGMENT_H__


# include <common/types.h>
# include <memory/IVirtualMemoryAddress.h>
# include <memory/IVirtualMemoryPage.h>


/* Access enum
 */
enum _vms_access {

  READ_ONLY = 0,
  WRITE_ONLY,
  READ_AND_WRITE
};
typedef enum _vms_access	vms_access_t;

/* Owner enum
 */
enum _vms_owner {

  KERNEL = 0,
  USER
};
typedef enum _vms_owner		vms_owner_t;

/* Memory object source type
 */
enum _vms_type {

  ANONYMOUS_SHARED = 0,
  ANONYMOUS_PRIVATE,
  NAMED_SHARED,
  NAMED_PRIVATE
};
typedef enum _vms_type vms_type_t;

/* IVirtualMemorySegment Object
 */
typedef struct _iVirtualMemorySegment
{
  void			(*init)					(struct _iVirtualMemorySegment* self, vaddr_t base_addr, offset_t size, 
								 IMemoryObject source, vms_access_t access, vms_owner_t owner,
								 vms_type_t type, IVirtualAddress addr_space);
  void			(*mapMemoryPageAtVirtualAddress)	(struct _iVirtualMemorySegment* self, IMemoryPage mem_page, 
								 vaddr_t vaddr);
  void			(*unmapVirtualMemoryPage)		(struct _iVirtualMemorySegment* self, IVirtualMemoryPage vmem_page);
  void			(*remapVirtualMemoryPage)		(struct _iVirtualMemorySegment* self, IVirtualMemoryPage vmem_page,
								 IMemoryPage mem_page);
  void			(*pageHasBeenSwappedIn)			(struct _iVirtualMemorySegment* self, IVirtualMemoryPage vmem_page);
  void			(*syncWithBackingStore)			(struct _iVirtualMemorySegment* self);
  //handle_page_fault_res_t	(*handlePageFault)
  vaddr_t		(*baseAddress)				(struct _iVirtualMemorySegment* self);
  size_t		(*size)					(struct _iVirtualMemorySegment* self);
  vms_access_t		(*access)				(struct _iVirtualMemorySegment* self);
  vms_owner_t		(*owner)				(struct _iVirtualMemorySegment* self);
  vms_type_t		(*type)					(struct _iVirtualMemorySegment* self);
  IVirtualAddressSpace	(*vspace)				(struct _iVirtualMemorySegment* self);
  unsigned		(*mappedPagesCount)			(struct _iVirtualMemorySegment* self);
  IVirtualMemoryPage	(*mappedPageAtIndex)			(struct _iVirtualMemorySegment* self, unsigned index);
  IVirtualMemoryPage	(*mappedPageByAddress)			(struct _iVirtualMemorySegment* self, vadd_t addr);

} __attribute__ ((packed)) * IVirtualMemorySegment;


#endif /* __KERNEL_MEMORY_I_VIRTUAL_MEMORY_SEGMENT_H__ */
