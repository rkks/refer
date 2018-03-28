/*
** _paging.c
** Setup the paging on IA32
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <arch/x86/x86_paging.h>
#include <base/string.h>


#define BINARY_SIZE	16384 /* TODO, SHOULD BE DYNAMIC ! */

struct _bootloader_memory_handler
{
  paddr_t	free_memory_base;
  paddr_t	free_memory_top;
  unsigned	used_page_count;
};

static BOOL _get_an_aligned_page_buf(struct _bootloader_memory_handler* pv_data, void** o_page)
{
  paddr_t ret = pv_data->free_memory_base + (pv_data->used_page_count * PAGESIZE);
  ASSERT(ret < pv_data->free_memory_top, "Bootloader out of memory");
  *o_page = (void*) ret;
  pv_data->used_page_count++;
  return YES;
}

static void	_init_paging(addr_space_def_t* addr_space_def)
{
  /* Init the bootloader handler
   */
  struct _bootloader_memory_handler bmem_handler;
  bmem_handler.free_memory_base = addr_space_def->map[LOW_MEMORY_IDENTIFY_MAPPING].paddr  + BINARY_SIZE;
  bmem_handler.free_memory_top = bmem_handler.free_memory_base + (addr_space_def->map[LOW_MEMORY_IDENTIFY_MAPPING].count * PAGESIZE);
  bmem_handler.used_page_count = 0;

  /* Init the page directory
   */
  x86_pde_t* kpd;
  (void) _get_an_aligned_page_buf(&bmem_handler, (void**) &kpd);
  memset(kpd, 0, PAGESIZE / sizeof(char));

  /* Map according to addr_space_def
   */
  for (unsigned i = 0; i < INITIAL_MAPPING_COUNT; i++)
    for (unsigned j = 0; j < addr_space_def->map[i].count; j++)
      {
	paddr_t paddr = addr_space_def->map[i].paddr + (j * PAGESIZE);
	vaddr_t vaddr = addr_space_def->map[i].vaddr + (j * PAGESIZE);
	x86_paging_map(kpd, paddr, vaddr, (BOOL (*)(void*, void**)) _get_an_aligned_page_buf, &bmem_handler);
      }

  /* Active paging
   *
   * TODO: comment !!!
   */
  x86_pdbr_t pdbr;
  pdbr.reserved_1 = 0;
  pdbr.reserved_2 = 0;
  pdbr.pwt = 0;
  pdbr.cache_disabled = 0;
  pdbr.pd_paddr = ((paddr_t)kpd) >> 12;

  /* Put the page directory address into CR3
   */
  x86_paging_write_cr3(pdbr);

  /* Set the paging bit in CR0 to 1
   */
  x86_paging_write_cr0(x86_paging_read_cr0() | 0x80000000);
}
