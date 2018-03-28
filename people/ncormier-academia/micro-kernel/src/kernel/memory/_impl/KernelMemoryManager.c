/*
** KernelMemoryManager.c
** Quick & dirty implementation
**
** This implementation used the page fault handler
** to get and map more memory in the kernel address
** space.
** This mecanism may not be safe in some case:
**	@ Interruption that need memomy immediatly
**	@ Piece of code where interruptions have been disabled
**	@ ...
**
** TODO:
**	@ Add a flag to k[page]alloc to solve the safitude problem ?
**	@ Slabs implementation
**	@ Quick list access (hash or multi-list by # contiguous page ?)
**	@ What are we supposed to do when there is no more memory ?
**	@ Create a mecanism to watch on kernel stack and kernel heap collison
**
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <memory/IKernelMemoryManager.h>
#include <common/addr_space_def.h>
#include <base/assert.h>
#include "KernelMemoryManager.h"


/* Forward declarations
 */
static void*	_kalloc(struct _kernelMemoryManager* self, size_t size);
static void	_kfree(struct _kernelMemoryManager* self, void* addr);
static void*	_kpagealloc(struct _kernelMemoryManager* self);
static void	_kpagefree(struct _kernelMemoryManager* self, void* page_addr);

/* Implementation
 */
static struct _kernelMemoryManager	_kernel_memory_manager_singleton;

IKernelMemoryManager	kernel_memory_manager_singleton(void)
{
  return (IKernelMemoryManager) &_kernel_memory_manager_singleton;
}

kstatus_t	kernel_memory_manager_setup(void)
{
  KernelMemoryManager	self = &_kernel_memory_manager_singleton;

  /* Init singleton object
   */
  self->kalloc = _kalloc;
  self->kfree = _kfree;
  self->kpagealloc = _kpagealloc;
  self->kpagefree = _kpagefree;

  /* Init members
   */
  SINGLY_LINKED_LIST_INIT(self->_freeBlockList);
  SINGLY_LINKED_LIST_INIT(self->_usedBlockList);
  SINGLY_LINKED_LIST_INIT(self->_freePageList);

  /* The bootloader have collected and mapped some page for our usage
   * So we can do some allocations before having a page fault
   * (page number depends of addr_space_def)
   */
  addr_space_def_t addr_space_def = default_addr_space_def();

  /* There is no _heapTop member because of the stack
   * (cf general todo list)
   */
  self->_usedHeapTop = self->_heapBase = addr_space_def.segments[HEAP_STACK_KSEG].base;
  self->_usedPageCollectionTop = self->_pageCollectionBase = addr_space_def.segments[KPAGE_COLLECTION_KSEG].base;
  self->_pageCollectionTop = addr_space_def.segments[KPAGE_COLLECTION_KSEG].top;

  return KS_SUCCESS;
}

static void*	_kalloc(struct _kernelMemoryManager* self, size_t size)
{
  /* First try to get a good sized block from the free list
   */
  SINGLY_LINKED_LIST_ITER(fblocklst) iter = SINGLY_LINKED_LIST_BEGIN(self->_freeBlockList);
  while (!SINGLY_LINKED_LIST_EOF(iter))
    {
      struct _block b = SINGLY_LINKED_LIST_VALUE(iter);
      if (b.size == size)
	{
	  /* There is a good size block, take it
	   */
	  SINGLY_LINKED_LIST_REMOVE(fblocklst, self->_freeBlockList, iter); /* Remove from free block list */
	  SINGLY_LINKED_LIST_PUSH(ublocklst, self->_usedBlockList, b); /* Add to used list */
	  return (void*) b.addr;
	}
      iter = SINGLY_LINKED_LIST_NEXT(iter);
    }

  /* There is no free block available, allocate a new block
   */
  struct _block new_block;
  new_block.addr = self->_usedHeapTop;
  new_block.size = size;
  SINGLY_LINKED_LIST_PUSH(ublocklst, self->_usedBlockList, new_block); /* Add to used list */  
  self->_usedHeapTop += size; /* Update used heap top address */

  return (void*) new_block.addr;
}

static void	_kfree(struct _kernelMemoryManager* self, void* addr)
{
  /* Browse used blocks
   */
  SINGLY_LINKED_LIST_ITER(fblocklst) iter = SINGLY_LINKED_LIST_BEGIN(self->_freeBlockList);
  while (!SINGLY_LINKED_LIST_EOF(iter))
    {
      struct _block b = SINGLY_LINKED_LIST_VALUE(iter);
      if (b.addr == (paddr_t) addr)
	{
	  /* Address found, remove it from used list and add it to free list
	   */
	  SINGLY_LINKED_LIST_REMOVE(ublocklst, self->_usedBlockList, iter); /* Remove from free page list */
	  SINGLY_LINKED_LIST_PUSH(fblocklst, self->_freeBlockList, b); /* Add to used list */
	  return;
	}
      iter = SINGLY_LINKED_LIST_NEXT(iter);
    }
  ASSERT(NO, "Unknown address");
}

static void*	_kpagealloc(struct _kernelMemoryManager* self)
{
  /* First try to get a page from the free list
   */
  if (!SINGLY_LINKED_LIST_EOF(self->_freePageList))
    {
      vaddr_t addr = SINGLY_LINKED_LIST_FIRST_VALUE(self->_freePageList);
      SINGLY_LINKED_LIST_POP(self->_freePageList); /* Remove from free page list */
      return (void*) addr;
    }

  /* There is no free page available, allocate a new page
   */
  void* ret = (void*) self->_usedPageCollectionTop;
  self->_usedPageCollectionTop += PAGESIZE; /* Update used heap top address */

  return ret;
}

static void	_kpagefree(struct _kernelMemoryManager* self, void* page_addr)
{
  /* Add page_addr to free page list
   */
  SINGLY_LINKED_LIST_PUSH(fpagelst, self->_freePageList, (vaddr_t) page_addr); /* Add to free list */  
}
