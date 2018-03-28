/*
** IPhysicalMemoryManager.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_PHYSICAL_MEMORY_MANANGER_H__
# define __KERNEL_MEMORY_I_PHYSICAL_MEMORY_MANANGER_H__


# include <common/types.h>
# include <memory/IPhysicalPage.h>


/* IPhysicalMemoryManager Object
 */
typedef struct _iPhysicalMemoryManager
{
  kstatus_t	(*retainNFreePages)		(struct _iPhysicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
  kstatus_t	(*retainNContiguousFreePages)	(struct _iPhysicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
  kstatus_t	(*releasePages)			(struct _iPhysicalMemoryManager* self, unsigned nbr_page, const IPhysicalPage* in_pages);
  unsigned	(*freePageCount)		(struct _iPhysicalMemoryManager* self);

} __attribute__ ((packed)) * IPhysicalMemoryManager;

/* Functions
 */
kstatus_t		physical_memory_manager_setup(paddr_t extended_mem_base, paddr_t extended_mem_top);
IPhysicalMemoryManager	physical_memory_manager_singleton(void);


#endif /* __KERNEL_MEMORY_I_PHYSICAL_MEMORY_MANANGER_H__ */
