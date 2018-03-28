/*
** AnonymousMemoryObjectPool.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/kalloc.h>
#include "AnonymousMemoryObjectPool.h"


/* Forward declaration
 */
static void		_init(struct _anonymousMemoryObjectPool* self);
static IMemoryObject	_pageableAnonymousMemoryObject(struct _anonymousMemoryObjectPool* self);
static IMemoryObject	_pageableAnonymousMemoryObjectWithDescriptor(struct _anonymousMemoryObjectPool* self, anon_desc_t desc);
static IMemoryObject	_nonPageableAnonymousMemoryObject(struct _anonymousMemoryObjectPool* self);
static IMemoryObject	_nonPageableAnonymousMemoryObjectWithDescriptor(struct _anonymousMemoryObjectPool* self, anon_desc_t desc);
static void		_scan(struct _anonymousMemoryObjectPool* self);

/* Implementation
 */
AnonymousMemoryObjectPool	alloc_memory_anonymous_object_pool(void)
{
  AnonymousMemoryObjectPool ret = kalloc(sizeof(struct _anonymousMemoryObjectPool));
  
  /* Initialize methods
   */
  ret->init = _init;
  ret->pageableAnonymousMemoryObject = _pageableAnonymousMemoryObject;
  ret->pageableAnonymousMemoryObjectWithDescriptor = _pageableAnonymousMemoryObjectWithDescriptor;
  ret->nonPageableAnonymousMemoryObject = _nonPageableAnonymousMemoryObject;
  ret->nonPageableAnonymousMemoryObjectWithDescriptor = nonPageableAnonymousMemoryObjectWithDescriptor;
  ret->scan = _scan;
  
  return ret;
}

static void	_init(struct _anonymousMemoryObjectPool* self)
{
  /* Initialize members
   */
  SINGLY_LINKED_LIST_INIT(self->_nonPageableAnonymousMemoryObjectList);
}

static IMemoryObject	_pageableAnonymousMemoryObject(struct _anonymousMemoryObjectPool* self)
{
  ASSERT(NO, "Not implemented yet");

  return 0x0;
}

static IMemoryObject	_pageableAnonymousMemoryObjectWithDescriptor(struct _anonymousMemoryObjectPool* self, anon_desc_t desc)
{
  ASSERT(NO, "Not implemented yet");

  return 0x0;
}

static IMemoryObject	_nonPageableAnonymousMemoryObject(struct _anonymousMemoryObjectPool* self)
{
  /* Allocate memory object and init it
   */
  NonPageableAnonymousMemoryObject ret = alloc_non_pageable_anonymous_memory_object();
  ret->init(ret);

  /* Add it to our list
   */
  SINGLY_LINKED_LIST_PUSH(npamolst, self->_nonPageableAnonymousMemoryObjectList, ret);

  return (IMemoryObject) ret;
}

static IMemoryObject	_nonPageableAnonymousMemoryObjectWithDescriptor(struct _anonymousMemoryObjectPool* self, anon_desc_t desc)
{
  ASSERT(NO, "Not implemented yet");

  return 0x0;
}

static void	_scan(struct _anonymousMemoryObjectPool* self)
{
  ASSERT(NO, "Not implemented yet");
}
