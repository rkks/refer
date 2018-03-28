/*
** PhysicalMemoryManager.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_IMPL_PHYSICAL_MEMORY_MANAGER_H__
# define __KERNEL_MEMORY_IMPL_PHYSICAL_MEMORY_MANAGER_H__


# include <memory/IPhysicalPage.h>
# include <base/queue.h> /* Singly linked list */
# include <common/types.h>


/* PhysicalMemoryManager Object
 */
typedef struct _physicalMemoryManager /* : IPhysicalMemoryManager */
{
  /* Kernel stack info
   */
  kstatus_t	(*retainNFreePages)		(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
  kstatus_t	(*retainNContiguousFreePages)	(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
  kstatus_t	(*releasePages)			(struct _physicalMemoryManager* self, unsigned nbr_page, const IPhysicalPage* in_pages);
  unsigned	(*freePageCount)		(struct _physicalMemoryManager* self);
  
  /* Private properties
   */
  SINGLY_LINKED_LIST(fpagelst, IPhysicalPage)	_freePageList;
  paddr_t					_extendedMemoryBase;
  paddr_t					_extendedMemoryTop;
  paddr_t					_usedExtendedMemoryTop;

} __attribute__ ((packed)) * PhysicalMemoryManager;


#endif /* __KERNEL_MEMORY_IMPL_PHYSICAL_MEMORY_MANAGER_H__ */
