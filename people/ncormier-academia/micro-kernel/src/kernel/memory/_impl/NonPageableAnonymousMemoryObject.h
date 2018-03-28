/*
** NonPageableAnonymousMemoryObject.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_H__
# define __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_H__


# include <memory/IMemoryObject.h>
# include <base/queue.h>


/* NonPageableAnonymousMemoryObjectPool Object
 */
typedef struct _nonPageableAnonymousMemoryObject /* : IMemoryObject */
{
  IMemoryPage	(*getPrivateMemoryPageAtOffset)		(struct _nonPageableAnonymousMemoryObject* self, offset_t offset);
  IMemoryPage	(*getSharedMemoryPageAtOffset)		(struct _nonPageableAnonymousMemoryObject* self, offset_t offset);
  void		(*release)				(struct _nonPageableAnonymousMemoryObject* self);
  void		(*retain)				(struct _nonPageableAnonymousMemoryObject* self);
  void		(*retrieveMemoryPageFromBackingStore)	(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);
  void		(*swapOutMemoryPage)			(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);
  void		(*syncMemoryPageWithBackingStore)	(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);

  /* Private members
   */
  void		(*init)					(struct _nonPageableAnonymousMemoryObject* self);
  void		(*initWithDescriptor)			(struct _nonPageableAnonymousMemoryObject* self, anon_desc_t desc);

  /* Properties
   */
  unsigned					_refCount;
  SINGLY_LINKED_LIST(amemplst, IMemoryPage)	_allocatedMemoryPageList;

  
} __attribute__ ((packed)) * NonPageableAnonymousMemoryObject;

/* Function
 */
NonPageableAnonymousMemoryObject	alloc_non_pageable_anonymous_memory_object(void);


#endif /* __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_H__ */
