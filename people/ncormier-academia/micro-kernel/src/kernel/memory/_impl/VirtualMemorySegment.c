/*
** VirtualMemorySegment.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/kalloc.h>
#include "VirtualMemorySegment.h"


/* Forward declarations
 */
static void			_init(struct _virtualMemorySegment* self, vaddr_t base_addr, offset_t size, 
				      IMemoryObject source, vms_access_t access, vms_owner_t owner, vms_type_t type, 
				      IVirtualAddress addr_space);
static void			_mapMemoryPageAtVirtualAddress(struct _virtualMemorySegment* self, IMemoryPage mem_page, vaddr_t vaddr);
static void			_unmapVirtualMemoryPage(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page);
static void			_remapVirtualMemoryPage(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page,
							IMemoryPage mem_page);
static void			_pageHasBeenSwappedIn(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page);
static void			_syncWithBackingStore(struct _virtualMemorySegment* self);
static vaddr_t			_baseAddress(struct _virtualMemorySegment* self);
static size_t			_size(struct _virtualMemorySegment* self);
static vms_access_t		_access(struct _virtualMemorySegment* self);
static vms_owner_t		_owner(struct _virtualMemorySegment* self);
static vms_type_t		_type(struct _virtualMemorySegment* self);
static IVirtualAddressSpace	_vspace(struct _virtualMemorySegment* self);
static unsigned			_mappedPagesCount(struct _virtualMemorySegment* self);
static IVirtualMemoryPage	_mappedPageAtIndex(struct _virtualMemorySegment* self, unsigned index);
static IVirtualMemoryPage	_mappedPageByAddress(struct _virtualMemorySegment* self, vadd_t addr);

/* Implementation
 */
VirtualMemorySegment	alloc_memory_virtual_segment(void)
{
  VirtualMemorySegment ret = kalloc(sizeof(struct _virtualMemorySegment));
  
  /* Init public members
   */
  ret->init = _init;
  ret->mapMemoryPageAtVirtualAddress = _mapMemoryPageAtVirtualAddress;
  ret->unmapVirtualMemoryPage = _unmapVirtualMemoryPage;
  ret->remapVirtualMemoryPage = _remapVirtualMemoryPage;
  ret->pageHasBeenSwappedIn = _pageHasBeenSwappedIn;
  ret->syncWithBackingStore = _syncWithBackingStore;
  ret->baseAddress = _baseAddress;
  ret->size = _size;
  ret->access = _access;
  ret->owner = _owner;
  ret->type = _type;
  ret->vspace = _vspace;
  ret->mappedPagesCount = _mappedPagesCount;
  ret->mappedPageAtIndex = _mappedPageAtIndex;
  ret->mappedPageByAddress = _mappedPageByAddress;

  return ret;
}

static void	_init(struct _virtualMemorySegment* self, vaddr_t base_addr, offset_t size, IMemoryObject source, 
		      vms_access_t access, vms_owner_t owner, vms_type_t type, IVirtualAddress addr_space)
{
  /* Init private members
   */
  self->_baseAddress = base_addr;
  self->_size = size;
  self->_source = source;
  self->_access = access;
  self->_owner = owner;
  self->_type = type;
  self->_vspace = addr_space;
  SINGLY_LINKED_LIST_INIT(self->_mappedPageList);

}

static void	_mapMemoryPageAtVirtualAddress(struct _virtualMemorySegment* self, IMemoryPage mem_page, vaddr_t vaddr)
{

}

static void	_unmapVirtualMemoryPage(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page)
{

}

static void	_remapVirtualMemoryPage(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page, IMemoryPage mem_page)
{

}

static void	_pageHasBeenSwappedIn(struct _virtualMemorySegment* self, IVirtualMemoryPage vmem_page)
{

}

static void	_syncWithBackingStore(struct _virtualMemorySegment* self)
{

}

static vaddr_t	_baseAddress(struct _virtualMemorySegment* self)
{

}

static size_t	_size(struct _virtualMemorySegment* self)
{
}

static vms_access_t	_access(struct _virtualMemorySegment* self)
{

}

static vms_owner_t	_owner(struct _virtualMemorySegment* self)
{

}

static vms_type_t	_type(struct _virtualMemorySegment* self)
{

}


static IVirtualAddressSpace	_vspace(struct _virtualMemorySegment* self)
{

}

static unsigned		_mappedPagesCount(struct _virtualMemorySegment* self)
{

}

static IVirtualMemoryPage	_mappedPageAtIndex(struct _virtualMemorySegment* self, unsigned index)
{

}

static IVirtualMemoryPage	_mappedPageByAddress(struct _virtualMemorySegment* self, vadd_t addr)
{
}
