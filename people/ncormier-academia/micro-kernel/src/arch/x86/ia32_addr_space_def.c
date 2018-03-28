/*
** ia32_addr_space_def.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <base/string.h> /* memcpy */
#include "ia32_addr_space_def.h"


/*
 * IA32 kernel virtual address space description:
 *
 *
 *	+------------------+ <- 0xFFF00000
 *	|                  |
 *	|   Kernel Stack   |
 *	|                  |
 *	/\/\/\/\/\/\/\/\/\/\	(512 Mo)
 *	/\/\/\/\/\/\/\/\/\/\
 *	|                  |
 *	|   Kernel Heap    |
 *	|                  |
 *	+------------------+ <- 0xDFF00000
 *	|                  |
 *	| Kernel Pages     |	(256 Mo)
 *	|      Collection  |
 *	|                  |
 *	+------------------+ <- 0xCFF00000
 *	|                  |
 *	|  Kernel Binary   |	(256 Mo)
 *	|                  |
 *	+------------------+ <- 0xBFF00000
 *	|                  |
 *	|      User        |	(~3 Go)
 *	|                  |
 *	+------------------+ <- 0x00000000
 */

#define VADDR_SEGMENT_KERNEL_LOW_BASE		0xBFF00000
#define VADDR_SEGMENT_KERNEL_LOW_TOP		0xCFF00000
#define VADDR_SEGMENT_KERNEL_PC_BASE		0xCFF00000
#define VADDR_SEGMENT_KERNEL_PC_TOP		0xDFF00000
#define VADDR_SEGMENT_KERNEL_HEAP_BASE		0xDFF00000
#define VADDR_SEGMENT_KERNEL_STACK_BOTTOM	0xFFF00000

#define PADDR_SEGMENT_LOW_MEMORY_BASE		0x10000
#define PADDR_SEGMENT_LOW_MEMORY_TOP		0xA0000
#define PADDR_SEGMENT_VGA_BASE			0xA0000
#define PADDR_SEGMENT_VGA_TOP			0xC0000
#define PADDR_INITIAL_PHY_PAGE_KPC_BASE		0x100000
#define PADDR_INITIAL_PHY_PAGE_KPC_TOP		0x120000
#define PADDR_INITIAL_PHY_PAGE_KHEAP_BASE	0x120000
#define PADDR_INITIAL_PHY_PAGE_KHEAP_TOP	0x132000
#define PADDR_INITIAL_PHY_PAGE_KSTACK_BASE	0x132000
#define PADDR_INITIAL_PHY_PAGE_KSTACK_TOP	0x136000

#define PAGE_NUMBER(base, top)	((top - base) / PAGESIZE)

static struct _addr_space_def	_addr_space =
  {
    .segments = {
      /* LOW_MEMORY_KSEG
       */
      { .base = VADDR_SEGMENT_KERNEL_LOW_BASE, .top = VADDR_SEGMENT_KERNEL_LOW_TOP}, 
      /* KPAGE_COLLECTION_KSEG
       */
      { .base = VADDR_SEGMENT_KERNEL_PC_BASE, .top = VADDR_SEGMENT_KERNEL_PC_TOP},
      /* HEAP_STACK_KSEG
       */
      { .base = VADDR_SEGMENT_KERNEL_HEAP_BASE, .top = VADDR_SEGMENT_KERNEL_STACK_BOTTOM}
    },
    . map = {
      /* LOW_MEMORY_IDENTIFY_MAPPING
       */
      { .paddr = PADDR_SEGMENT_LOW_MEMORY_BASE,
	.vaddr = PADDR_SEGMENT_LOW_MEMORY_BASE,
	.count = PAGE_NUMBER(PADDR_SEGMENT_LOW_MEMORY_BASE, PADDR_SEGMENT_LOW_MEMORY_TOP)
      },
      /* LOW_MEMORY_MAPPING
       */
      { .paddr = PADDR_SEGMENT_LOW_MEMORY_BASE,
	.vaddr = VADDR_SEGMENT_KERNEL_LOW_BASE,
	.count = PAGE_NUMBER(PADDR_SEGMENT_LOW_MEMORY_BASE, PADDR_SEGMENT_LOW_MEMORY_TOP)
      },
      /* VGA_IDENTIFY_MAPPING
       */
      { .paddr = PADDR_SEGMENT_VGA_BASE,
	.vaddr = PADDR_SEGMENT_VGA_BASE,
	.count = PAGE_NUMBER(PADDR_SEGMENT_VGA_BASE, PADDR_SEGMENT_VGA_TOP)
      },
      /* KPAGE_COLLECTION_MAPPING
       */
      { .paddr = PADDR_INITIAL_PHY_PAGE_KPC_BASE,
	.vaddr = VADDR_SEGMENT_KERNEL_PC_BASE,
	.count = PAGE_NUMBER(PADDR_INITIAL_PHY_PAGE_KPC_BASE, PADDR_INITIAL_PHY_PAGE_KPC_TOP)
      },
      /* KHEAP_MAPPING
       */
      { .paddr = PADDR_INITIAL_PHY_PAGE_KHEAP_BASE,
	.vaddr = VADDR_SEGMENT_KERNEL_HEAP_BASE,
	.count = PAGE_NUMBER(PADDR_INITIAL_PHY_PAGE_KHEAP_BASE, PADDR_INITIAL_PHY_PAGE_KHEAP_TOP)
      },
      /* KSTACK_MAPPING
       */
      { .paddr = PADDR_INITIAL_PHY_PAGE_KSTACK_BASE,
	.vaddr = VADDR_SEGMENT_KERNEL_STACK_BOTTOM - (PADDR_INITIAL_PHY_PAGE_KSTACK_TOP -
						      PADDR_INITIAL_PHY_PAGE_KSTACK_BASE),
	.count = PAGE_NUMBER(PADDR_INITIAL_PHY_PAGE_KSTACK_BASE, PADDR_INITIAL_PHY_PAGE_KSTACK_TOP)
      }
    }
  };

addr_space_def_t	default_addr_space_def(void)
{
  return _addr_space;
}
