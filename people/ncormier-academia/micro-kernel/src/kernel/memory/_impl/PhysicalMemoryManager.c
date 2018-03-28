/*
** PhysicalMemoryManager.c
** Quick & dirty implementation 
**
**
** TODO:	
**	@ Anti fragmentation mecanism
**	@ Quick list access (hash or multi-list by # contiguous page ?) 
**	@ Keep a used page list to check caller input
**	@ What suppose to do when there is no more physical memory ?
**		-> Talk with VM-pager to prevent that
**		-> return KS_FAILED
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/


#include <memory/IPhysicalMemoryManager.h>
#include <common/addr_space_def.h>
#include <base/assert.h>
#include "PhysicalMemoryManager.h"

/* Forward declarations
 */
static kstatus_t	retain_n_free_pages(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
static kstatus_t	retain_n_contiguous_free_pages(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages);
static kstatus_t	release_pages(struct _physicalMemoryManager* self, unsigned nbr_page, const IPhysicalPage* in_pages);
static unsigned		free_page_count(struct _physicalMemoryManager* self);

/* Implementation
 */
static struct _physicalMemoryManager	_physical_memory_manager_singleton;

IPhysicalMemoryManager	physical_memory_manager_singleton(void)
{
  return (IPhysicalMemoryManager) &_physical_memory_manager_singleton;
}

kstatus_t		physical_memory_manager_setup(paddr_t extended_mem_base, paddr_t extended_mem_top)
{
  PhysicalMemoryManager	self = &_physical_memory_manager_singleton;

  /* Init singleton object
   */
  self->retainNFreePages = retain_n_free_pages;
  self->retainNContiguousFreePages = retain_n_contiguous_free_pages;
  self->releasePages = release_pages;
  self->freePageCount = free_page_count;
  self->_extendedMemoryBase = extended_mem_base;
  self->_extendedMemoryTop = extended_mem_top;

  /* The bootloader have already collected and mapped some physical page
   * (from the extended memory)
   * We have to browse the address space definition to find how much memory 
   * the bootloader have assigned
   * We assume that collected pages are contiguous and the first collected
   * page start at the base of extended memory
   */
  addr_space_def_t addr_space_def = default_addr_space_def();
  paddr_t high_collected_page_paddr = 0;
  for (unsigned i = 0; i < INITIAL_MAPPING_COUNT; i++)
    {
      paddr_t paddr = addr_space_def.map[i].paddr + (addr_space_def.map[i].count * PAGESIZE);
      if (paddr > high_collected_page_paddr) high_collected_page_paddr = paddr;
    }
  ASSERT(high_collected_page_paddr <= extended_mem_base, "Bootloader should have collected/mapped few physical pages");
  self->_usedExtendedMemoryTop = high_collected_page_paddr + PAGESIZE; /* Now we know where are free physical pages */

  /* Init members
   */
  SINGLY_LINKED_LIST_INIT(self->_freePageList);

  return KS_SUCCESS;
}

static kstatus_t	retain_helper(struct _physicalMemoryManager* self, unsigned nbr_page, 
				      IPhysicalPage* out_pages, BOOL try_to_search_in_free_page_list)
{
  for (unsigned i = 0; i < nbr_page; i++)
    {
      IPhysicalPage page_to_give = 0x0;

      /* First try to get from free list
       */
      if (try_to_search_in_free_page_list)
	page_to_give = SINGLY_LINKED_LIST_FIRST_VALUE(self->_freePageList);
      if (page_to_give) /* There is a free page available */
	{
	  SINGLY_LINKED_LIST_POP(self->_freePageList); /* Remove from free page list */
	}
      else /* Free page list is empty */
	{
	  /* Move the kernel extended memory top address
	   */
	  ASSERT(self->_extendedMemoryTop > self->_usedExtendedMemoryTop, "Extended memory full");
	  page_to_give = physical_page_alloc(); /* Alloc a new physical page object */
	  page_to_give->initWithPhysicalAddress(page_to_give, self->_usedExtendedMemoryTop); /* Associate top address with page */
	  self->_usedExtendedMemoryTop += PAGESIZE;
	}
      ASSERT(page_to_give != 0x0, "page_to_give is null");

      /* Insert in caller output
       */
      out_pages[i] = page_to_give;
    }

  return KS_SUCCESS;
}

static kstatus_t	retain_n_free_pages(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages)
{
  return retain_helper(self, nbr_page, out_pages, YES);
}

static kstatus_t	retain_n_contiguous_free_pages(struct _physicalMemoryManager* self, unsigned nbr_page, IPhysicalPage* out_pages)
{
  return retain_helper(self, nbr_page, out_pages, NO);
}

static kstatus_t	release_pages(struct _physicalMemoryManager* self, unsigned nbr_page, const IPhysicalPage* in_pages)
{
  for (unsigned i = 0; i < nbr_page; i++)
    SINGLY_LINKED_LIST_PUSH(fpagelst, self->_freePageList, in_pages[i]); /* Add to free list */

  return KS_SUCCESS;
}

static unsigned		free_page_count(struct _physicalMemoryManager* self)
{
  unsigned ret = (self->_extendedMemoryTop - self->_usedExtendedMemoryTop) / PAGESIZE;
  SINGLY_LINKED_LIST_ITER(fpagelst) iter = SINGLY_LINKED_LIST_BEGIN(self->_freePageList);
  while (!SINGLY_LINKED_LIST_EOF(iter))
    {
      ret++;
      iter = SINGLY_LINKED_LIST_NEXT(iter);
    }

  return ret;
}
