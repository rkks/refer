/*
** AnonymousMemoryObjectPool.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_POOL_H__
# define __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_POOL_H__


# include <memory/IAnonymousMemoryObjectPool.h>
# include "NonPageableAnonymousMemoryObject.h"


/* AnonymousMemoryObjectPool Object
 */
typedef struct _anonymousMemoryObjectPool
{
  void		(*init)							(struct _anonymousMemoryObjectPool* self);
  IMemoryObject	(*pageableAnonymousMemoryObject)			(struct _anonymousMemoryObjectPool* self);
  IMemoryObject	(*pageableAnonymousMemoryObjectWithDescriptor)		(struct _anonymousMemoryObjectPool* self, anon_desc_t desc);
  IMemoryObject	(*nonPageableAnonymousMemoryObject)			(struct _anonymousMemoryObjectPool* self);
  IMemoryObject	(*nonPageableAnonymousMemoryObjectWithDescriptor)	(struct _anonymousMemoryObjectPool* self, anon_desc_t desc);
  void		(*scan)							(struct _anonymousMemoryObjectPool* self);

  /* Private members
   */
  SINGLY_LINKED_LIST(npamolst, NonPageableAnonymousMemoryObject)	_nonPageableAnonymousMemoryObjectList;


} __attribute__ ((packed)) * AnonymousMemoryObjectPool;

/* Function
 */
AnonymousMemoryObjectPool	alloc_memory_anonymous_object_pool(void);


#endif /* __KERNEL_MEMORY__IMPL_ANONYMOUS_MEMORY_OBJECT_POOL_H__ */
