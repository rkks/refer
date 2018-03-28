/*
** x86_paging.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <base/assert.h>
#include "x86_paging.h"


BOOL	x86_paging_map(x86_pde_t* pd, paddr_t paddr, 
		       vaddr_t vaddr, BOOL (*get_an_aligned_page_buf)(void* pv_data, void** o_page),
		       void* pv_data)
{
  /* Insert or get the good page table
   */
  unsigned pdidx = VADDR_TO_PDIDX(vaddr);
  unsigned ptidx = VADDR_TO_PTIDX(vaddr);
  x86_pte_t* pt = 0x0;
  if (!pd[pdidx].prop.present) /* Page is not present */
    {
      /* Try to get a page buf
       */
      void* new_page;
      if (!get_an_aligned_page_buf(pv_data, &new_page)) return NO;
      pt = (x86_pte_t*) new_page;

      /* Set to 0
       * TODO: use an asm memset
       */
      for (unsigned i = 0; i < 1024; i++) pt[i].value = 0;

      /* TODO: comment !!!!
       */
      pd[pdidx].prop.present = 1;
      pd[pdidx].prop.rw = 1; 
      pd[pdidx].prop.user = 0;
      pd[pdidx].prop.pt_paddr = ((paddr_t)pt) >> 12;
    }
  else /* Page is present */
    pt = (x86_pte_t*) (pd[pdidx].prop.pt_paddr << 12);

  /* Ok, now map the page in the page table
   *
   * TODO: comment !!!!
   *
   */
  pt[ptidx].prop.present = 1;
  pt[ptidx].prop.rw = 1;
  pt[ptidx].prop.user = 0;
  pt[ptidx].prop.paddr = paddr >> 12;

  return YES;
}

void	x86_paging_write_cr3(x86_pdbr_t pdbr)
{
  __asm__ volatile("movl %0, %%cr3" : : "r"(pdbr));
}

void	x86_paging_write_cr0(register_t r)
{
  __asm__ volatile("movl %0, %%cr0" : : "r"(r));
}

register_t	x86_paging_read_cr0(void)
{
  register_t ret;
  __asm__ volatile("movl %%cr0, %0" : "=r"(ret));

  return ret;
}
