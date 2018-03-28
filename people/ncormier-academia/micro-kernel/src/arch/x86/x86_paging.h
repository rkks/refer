/*
** x86_paging.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __ARCH_X86_PAGING_H__
# define __ARCH_X86_PAGING_H__


# include <base/types.h>
# include "x86_paging_types.h"


BOOL		x86_paging_map(x86_pde_t* pd, paddr_t paddr, vaddr_t vaddr, 
			       BOOL (*get_an_aligned_page_buf_callback)(void* pv_data, void** o_page),
			       void* callback_pv_data);
void		x86_paging_write_cr3(x86_pdbr_t pdbr);
void		x86_paging_write_cr0(register_t r);
register_t	x86_paging_read_cr0(void);


#endif /* __ARCH_X86_PAGING_H__ */
