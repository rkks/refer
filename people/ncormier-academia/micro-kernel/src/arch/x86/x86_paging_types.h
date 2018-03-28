/*
** x86_paging_types.h
** X86 Paging Types
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __ARCH_X86_PAGING_TYPES_H_
# define __ARCH_X86_PAGING_TYPES_H__


# include <base/types.h>


/* Index makers
 *
 *	        Linear address
 *    ___________________________________
 *   |  directory  |  table  |  offset  |
 *   -----------------------------------
 *   31         22 21     12 11        0
 *
 *	31-22: 10 bits = index in page directory
 *	21-12: 10 bits = index in page table
 *	11-00: 12 bits = offset in physical page
 */
#define VADDR_TO_PDIDX(vaddr)	(((unsigned)(vaddr)) >> 22)
#define VADDR_TO_PTIDX(vaddr)	((((unsigned)(vaddr)) >> 12) & 0x3ff)
#define VADDR_TO_POFF(vaddr)	(((unsigned)(vaddr)) & PAGESIZE)

/* Page directory entry
 */
union _x86_pde
{
  unsigned	value;
  struct
  {
    unsigned	present		: 1; /* 1=PT mapped */
    unsigned	rw		: 1; /* 0=read-only, 1=read/write */
    unsigned	user		: 1; /* 0=supervisor, 1=user */
    unsigned	write_through	: 1; /* 0=write-back, 1=write-through */
    unsigned	cache_disabled	: 1; /* 1=cache disabled */
    unsigned	accessed	: 1; /* 1=read/write access since last clear */
    unsigned	reserved	: 1; /* Intel reserved */
    unsigned	page_size	: 1; /* 0=4kB, 1=4MB or 2MB (depending on PAE) */
    unsigned	global_page	: 1; /* Ignored (Intel reserved) */
    unsigned	os_usage	: 3;
    unsigned	pt_paddr	: 20;
  }		prop;
} __attribute__ ((packed));
typedef union _x86_pde	x86_pde_t;

/* Page table entry
 */
union _x86_pte
{
  unsigned	value;
  struct
  {
    unsigned	present		: 1; /* 1=PT mapped */
    unsigned	rw		: 1; /* 0=read-only, 1=read/write */
    unsigned	user		: 1; /* 0=supervisor, 1=user */
    unsigned	write_through	: 1; /* 0=write-back, 1=write-through */
    unsigned	cache_disabled	: 1; /* 1=cache disabled */
    unsigned	accessed	: 1; /* 1=read/write access since last clear */
    unsigned	dirty		: 1; /* 1=write access since last clear */
    unsigned	reserved	: 1; /* Intel reserved */
    unsigned	global_page	: 1; /* 1=No TLB invalidation upon cr3 switch
					(when PG set in cr4) */
    unsigned	os_usage	: 3;
    unsigned	paddr		: 20;
  }		prop;
} __attribute__ ((packed));
typedef union _x86_pte	x86_pte_t;

struct _x86_pdbr
{
  unsigned	reserved_1	: 3; /* Intel reserved */
  unsigned	pwt		: 1;
  unsigned	cache_disabled	: 1; /* 1=cache disabled */
  unsigned	reserved_2	: 7; /* Intel reserved */
  unsigned	pd_paddr	: 20;
} __attribute__ ((packed));
typedef struct _x86_pdbr x86_pdbr_t;


#endif /* __ARCH_X86_PAGING_TYPES_H__ */
