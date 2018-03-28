/*
** IMemoryObjectManager.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_MEMORY_OBJECT_MANAGER_H__
# define __KERNEL_MEMORY_I_MEMORY_OBJECT_MANAGER_H__


# include "IAnonymousMemoryObjectPool.h"


/* IMemoryObjectManager Object
 */
typedef struct _iMemoryObjectManager
{
  IAnonymousMemoryObjectPool	(*anonymousMemoryObjectPool)	(struct _iMemoryObjectManager* self);
  /* namedMemoryObjectPool */
  void				(*scan)				(struct _iMemoryObjectManager* self);

} __attribute__ ((packed)) * IMemoryObjectManager;

/* Functions
 */
kstatus_t		object_memory_manager_setup(void);
IMemoryObjectManager	object_memory_manager_singleton(void);


#endif /* __KERNEL_MEMORY_I_MEMORY_OBJECT_MANAGER_H__ */
