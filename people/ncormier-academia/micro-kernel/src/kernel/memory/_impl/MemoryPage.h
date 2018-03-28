/*
** MemoryPage.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_MEMORY_PAGE_H__
# define __KERNEL_MEMORY__IMPL_MEMORY_PAGE_H__


# include <memory/IMemoryPage.h>
# include <base/queue.h>


/* MemoryPage Object
 */
typedef struct _memoryPage
{
  void			(*syncWithBackingStore)		(struct _memoryPage* self);
  void			(*retain)			(struct _memoryPage* self);
  void			(*release)			(struct _memoryPage* self);
  void			(*pageOut)			(struct _memoryPage* self);
  void			(*pageIn)			(struct _memoryPage* self);
  void			(*lock)				(struct _memoryPage* self);
  void			(*unlock)			(struct _memoryPage* self);
  unsigned		(*mappedVirtualPagesCount)	(struct _memoryPage* self);
  IVirtualMemoryPage	(*mappedVirtualPageAtIndex)	(struct _memoryPage* self, unsigned idx);
  IPhysicalPage		(*physicalPage)			(struct _memoryPage* self);
  offset_t		(*sourceOffset)			(struct _memoryPage* self);
  IMemoryObject		(*source)			(struct _memoryPage* self);
  BOOL			(*isPresent)			(struct _memoryPage* self);
  BOOL			(*isDirty)			(struct _memoryPage* self);

  /* Private members
   */
  unsigned						_refCount; /* TODO, replace by refcount object */
  IPhysicalPage						_physicalPage;
  offset_t						_sourceOffset;
  IMemoryObject						_source;
  SINGLY_LINKED_LIST(vmplst, IVirtualMemoryPage)	_virtualMemoryPageList;						

} __attribute__ ((packed)) * MemoryPage;

/* Functions
 */
MemoryPage	alloc_memory_page(void);

#endif /* __KERNEL_MEMORY__IMPL_MEMORY_PAGE_H__ */
