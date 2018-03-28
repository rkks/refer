/*
** IMemoryObject.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_MEMORY_OBJECT_H__
# define __KERNEL_MEMORY_I_MEMORY_OBJECT_H__


# include <common/types.h>


/* IMemoryObject Object
 */
typedef struct _iMemoryObject
{
  IMemoryPage	(*getPrivateMemoryPageAtOffset)		(struct _iMemoryObject* self, offset_t offset);
  IMemoryPage	(*getSharedMemoryPageAtOffset)		(struct _iMemoryObject* self, offset_t offset);
  void		(*release)				(struct _iMemoryObject* self);
  void		(*retain)				(struct _iMemoryObject* self);
  void		(*retrieveMemoryPageFromBackingStore)	(struct _iMemoryObject* self, IMemoryPage page);
  void		(*swapOutMemoryPage)			(struct _iMemoryObject* self, IMemoryPage page);
  void		(*syncMemoryPageWithBackingStore)	(struct _iMemoryObject* self, IMemoryPage page);
  memobj_type_t	(*type)					(struct _iMemoryObject* self);

} __attribute__ ((packed)) * IMemoryObject;


#endif /* __KERNEL_MEMORY_I_MEMORY_OBJECT_H__ */
