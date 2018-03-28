/*
** KernelMemoryManager.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_KERNEL_MEMORY_MANAGER_H__
# define __KERNEL_MEMORY__IMPL_KERNEL_MEMORY_MANAGER_H__


#include <common/types.h>
#include <base/queue.h>


struct _block
{
  vaddr_t	addr;
  size_t	size;
};

/* KernelMemoryManager Object
 */
typedef struct _kernelMemoryManager /* : IKernelMemoryManager */
{
  void*	(*kalloc)	(struct _kernelMemoryManager* self, size_t size);
  void	(*kfree)	(struct _kernelMemoryManager* self, void* addr);
  void*	(*kpagealloc)	(struct _kernelMemoryManager* self);
  void	(*kpagefree)	(struct _kernelMemoryManager* self, void* page_addr);

  /* Private members
   */
  vaddr_t					_heapBase;
  vaddr_t					_usedHeapTop;
  vaddr_t					_pageCollectionBase;
  vaddr_t					_pageCollectionTop;
  vaddr_t					_usedPageCollectionTop;
  SINGLY_LINKED_LIST(fblocklst, struct _block)	_freeBlockList; /* Free blocks (kalloc usage) */
  SINGLY_LINKED_LIST(ublocklst, struct _block)	_usedBlockList; /* Used blocks (kalloc usage) */
  SINGLY_LINKED_LIST(fpagelst, vaddr_t)		_freePageList; /* Free pages (kpagealloc usage) */

} __attribute__ ((packed)) * KernelMemoryManager;


#endif /* __KERNEL_MEMORY__IMPL_KERNEL_MEMORY_MANAGER_H__ */
