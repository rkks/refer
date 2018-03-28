/*
** MemoryPage.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/kalloc.h>
#include <base/assert.h>
#include "MemoryPage.h"


/* Forward declaration
 */
static void			_syncWithBackingStore(struct _memoryPage* self);
static void			_retain(struct _memoryPage* self);
static void			_release(struct _memoryPage* self);
static void			_pageOut(struct _memoryPage* self);
static void			_pageIn(struct _memoryPage* self);
static void			_lock(struct _memoryPage* self);
static void			_unlock	(struct _memoryPage* self);
static unsigned			_mappedVirtualPagesCount(struct _memoryPage* self);
static IVirtualMemoryPage	_mappedVirtualPageAtIndex(struct _memoryPage* self, unsigned idx);
static IPhysicalPage		_physicalPage(struct _memoryPage* self);
static offset_t			_sourceOffset(struct _memoryPage* self);
static IMemoryObject		_source(struct _memoryPage* self);
static BOOL			_isPresent(struct _memoryPage* self);
static BOOL			_isDirty(struct _memoryPage* self);

/* Implementation
 */
MemoryPage	alloc_memory_page(void)
{
  MemoryPage ret = kalloc(sizeof(struct _memoryPage));
  
  /* Initialize methods
   */
  ret->syncWithBackingStore = _syncWithBackingStore;
  ret->retain = _retain;
  ret->release = _release;
  ret->pageOut = _pageOut;
  ret->pageIn = _pageIn;
  ret->lock = _lock;
  ret->unlock = _unlock;
  ret->mappedVirtualPagesCount = _mappedVirtualPagesCount;
  ret->mappedVirtualPageAtIndex = _mappedVirtualPageAtIndex;
  ret->physicalPage = _physicalPage;
  ret->sourceOffset = _sourceOffset;
  ret->source = _source;
  ret->isPresent = _isPresent;
  ret->isDirty = _isDirty;

  return ret;
}

static void	_syncWithBackingStore(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static void	_retain(struct _memoryPage* self)
{
  self->_refCount++;
}

static void	_release(struct _memoryPage* self)
{
  self->_refCount--;
}

static void	_pageOut(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static void	_pageIn(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static void	_lock(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static void	_unlock	(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static unsigned	_mappedVirtualPagesCount(struct _memoryPage* self)
{
  unsigned ret = 0;
  
  /* Browse virtual page list
   */
  SINGLY_LINKED_LIST_ITER(vmplst) iter = SINGLY_LINKED_LIST_BEGIN(self->_virtualMemoryPageList);
  while (!SINGLY_LINKED_LIST_EOF(iter))
    {
      ret++;
      iter = SINGLY_LINKED_LIST_NEXT(iter);
    }

  return ret;
}

static IVirtualMemoryPage	_mappedVirtualPageAtIndex(struct _memoryPage* self, unsigned idx)
{
  /* Browse virtual page list
   */
  SINGLY_LINKED_LIST_ITER(vmplst) iter = SINGLY_LINKED_LIST_BEGIN(self->_virtualMemoryPageList);
  while (!SINGLY_LINKED_LIST_EOF(iter))
    {
      if (!idx) return SINGLY_LINKED_LIST_VALUE(iter);
      iter = SINGLY_LINKED_LIST_NEXT(iter);
    }
  ASSERT(NO, "Too high index");
}

static IPhysicalPage	_physicalPage(struct _memoryPage* self)
{
  return self->_physicalPage;
}

static offset_t	_sourceOffset(struct _memoryPage* self)
{
  return self->_sourceOffset;
}

static IMemoryObject	_source(struct _memoryPage* self)
{
  return self->_source;
}

static BOOL	_isPresent(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}

static BOOL	_isDirty(struct _memoryPage* self)
{
  ASSERT(NO, "Not implemented yet");
}
