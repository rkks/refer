/*
** NonPageableAnonymousMemoryObject.c
** Temporary implementation
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/kalloc.h>
#include <base/assert.h>
#include <memory/IPhysicalMemoryManager.h>
#include "NonPageableAnonymousMemoryObject.h"
#include "MemoryPage.h"


/* Forward declarations
 */
static IMemoryPage	_getPrivateMemoryPageAtOffset(struct _nonPageableAnonymousMemoryObject* self, offset_t offset);
static IMemoryPage	_getSharedMemoryPageAtOffset(struct _nonPageableAnonymousMemoryObject* self, offset_t offset);
static void		_release(struct _nonPageableAnonymousMemoryObject* self);
static void		_retain(struct _nonPageableAnonymousMemoryObject* self);
static void		_retrieveMemoryPageFromBackingStore(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);
static void		_swapOutMemoryPage(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);
static void		_syncMemoryPageWithBackingStore(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page);

static void		_init(struct _nonPageableAnonymousMemoryObject* self);
static void		_initWithDescriptor(struct _nonPageableAnonymousMemoryObject* self, anon_desc_t desc);

/* Implementation
 */
NonPageableAnonymousMemoryObject	alloc_non_pageable_anonymous_memory_object(void)
{
  NonPageableAnonymousMemoryObject ret = (NonPageableAnonymousMemoryObject)
    kalloc(sizeof(struct _nonPageableAnonymousMemoryObject));

  /* Init methods
   */
  ret->getPrivateMemoryPageAtOffset = _getPrivateMemoryPageAtOffset;
  ret->getSharedMemoryPageAtOffset = _getSharedMemoryPageAtOffset;
  ret->release = _release;
  ret->retain = _retain;
  ret->retrieveMemoryPageFromBackingStore = _retrieveMemoryPageFromBackingStore;
  ret->swapOutMemoryPage = _swapOutMemoryPage;
  ret->syncMemoryPageWithBackingStore = _syncMemoryPageWithBackingStore;
  ret->init = _init;
  ret->initWithDescriptor = _initWithDescriptor;

  return ret;
}

static IMemoryPage	_getPrivateMemoryPageAtOffset(struct _nonPageableAnonymousMemoryObject* self, offset_t /* offset */)
{
  IPhysicalMemoryManager pmm = physical_memory_manager_singleton();
  PhysicalPage ppage;
  kstatus_t s = pmm->retainNFreePages(pmm, 1, &ppage);
  
  /* What are we suppose to do when there is no free physical page ?
   */
  ASSERT(s == KS_SUCCESS, "retainNFreePages() failed");
  
  /* Allocate memory page
   */
  MemoryPage ret = alloc_memory_page();
  ret->_physicalPage = ppage; /* Assign physical page */
  ret->_source = (IMemoryObject) self;
  ret->_sourceOffset = ~0; /* Unsignificant offset */

  /* Add memory page to our list
   */
  SINGLY_LINKED_LIST_PUSH(vmplst, self->_virtualMemoryPageList, ret);

  return (IMemoryPage) ret;
}

static IMemoryPage	_getSharedMemoryPageAtOffset(struct _nonPageableAnonymousMemoryObject* self, offset_t offset)
{
  ASSERT(NO, "Not implemented yet");

  return 0x0;
}

static void		_release(struct _nonPageableAnonymousMemoryObject* self)
{
  ASSERT(self->_refCount, "Call retain before release an object");
  self->_refCount--;
}

static void		_retain(struct _nonPageableAnonymousMemoryObject* self)
{
  self->_refCount++;
}

static void		_retrieveMemoryPageFromBackingStore(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page)
{
  ASSERT(NO, "Not implemented yet");
}

static void		_swapOutMemoryPage(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page)
{
  ASSERT(NO, "Not implemented yet");
}

static void		_syncMemoryPageWithBackingStore(struct _nonPageableAnonymousMemoryObject* self, IMemoryPage page)
{
  ASSERT(NO, "Not implemented yet");
}

static void		_init(struct _nonPageableAnonymousMemoryObject* self)
{
  /* Initializae properties
   */
  self->_refCount = 1;
  SINGLY_LINKED_LIST_INIT(self->_allocatedMemoryPageList);
}

static void		_initWithDescriptor(struct _nonPageableAnonymousMemoryObject* self, anon_desc_t desc)
{
  ASSERT(NO, "Not implemented yet");
}
