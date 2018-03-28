/*
** IAnonymousMemoryObjectPool.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_ANONYMOUS_MEMORY_OBJECT_POOL_H__
# define __KERNEL_MEMORY_I_ANONYMOUS_MEMORY_OBJECT_POOL_H__


# include "IMemoryObject.h"


/* Anonymous descriptor
 */
struct _anon_desc
{
  unsigned d;
};
typedef struct _anon_desc	anon_desc_t;

/* IAnonymousMemoryObjectPool Object
 */
typedef struct _iAnonymousMemoryObjectPool
{
  void		(*init)							(struct _iAnonymousMemoryObjectPool* self);
  IMemoryObject	(*pageableAnonymousMemoryObject)			(struct _iAnonymousMemoryObjectPool* self);
  IMemoryObject	(*pageableAnonymousMemoryObjectWithDescriptor)		(struct _iAnonymousMemoryObjectPool* self, anon_desc_t desc);
  IMemoryObject	(*nonPageableAnonymousMemoryObject)			(struct _iAnonymousMemoryObjectPool* self);
  IMemoryObject	(*nonPageableAnonymousMemoryObjectWithDescriptor)	(struct _iAnonymousMemoryObjectPool* self, anon_desc_t desc);
  void		(*scan)							(struct _iAnonymousMemoryObjectPool* self);

} __attribute__ ((packed)) * IAnonymousMemoryObjectPool;


#endif /* __KERNEL_MEMORY_I_ANONYMOUS_MEMORY_OBJECT_POOL_H__ */
