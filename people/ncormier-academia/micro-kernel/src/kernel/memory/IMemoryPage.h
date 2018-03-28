/*
** IMemoryPage.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_MEMORY_PAGE_H__
# define __KERNEL_MEMORY_I_MEMORY_PAGE_H__


# include <common/types.h>
# include <memory/IVirtualMemoryPage.h>
# include <memory/IPhysicalPage.h>


/* IMemoryPage Object
 */
typedef struct _iMemoryPage
{
  void			(*syncWithBackingStore)		(struct _iMemoryPage* self);
  void			(*retain)			(struct _iMemoryPage* self);
  void			(*release)			(struct _iMemoryPage* self);
  void			(*pageOut)			(struct _iMemoryPage* self);
  void			(*pageIn)			(struct _iMemoryPage* self);
  void			(*lock)				(struct _iMemoryPage* self);
  void			(*unlock)			(struct _iMemoryPage* self);
  unsigned		(*mappedVirtualPagesCount)	(struct _iMemoryPage* self);
  IVirtualMemoryPage	(*mappedVirtualPageAtIndex)	(struct _iMemoryPage* self, unsigned idx);
  IPhysicalPage		(*physicalPage)			(struct _iMemoryPage* self);
  offset_t		(*sourceOffset)			(struct _iMemoryPage* self);
  IMemoryObject		(*source)			(struct _iMemoryPage* self);
  BOOL			(*isPresent)			(struct _iMemoryPage* self);
  BOOL			(*isDirty)			(struct _iMemoryPage* self);

} __attribute__ ((packed)) * IMemoryPage;


#endif /* __KERNEL_MEMORY_I_MEMORY_PAGE_H__ */
