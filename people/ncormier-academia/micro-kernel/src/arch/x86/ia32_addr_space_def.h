/*
** ia32_addr_space_def.h
**
** During the boot step we need the definiton of the
** kernel virtual address space.
** This file contains information about the organisation
** of this virtual address space and physical address space.
** Each segment must have few physical mapped pages to 
** complete the boot step. Those pages, how they are mapped
** are discribed here.
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** 
** 
** The kernel virtual address space is organized like that:
** Schemes also show the mapping between physical and 
** virtual address space after the booloader initialization
** 
** 
**         
**      Physical Address (low)
**      ======================
**
**
**                                 PHYSICAL ADDRESS SPACE
**                    
**   0x10000                                                                 0xA0000    
**      +------------------+--------------+-------------------------------------+
**      |  bootloader.bin  |  kernel.bin  |  bootloader usage (page dir/table)  |
**      +------------------+--------------+-------------------------------------+
**      |                                                                       |
**      |                                                                       |
**      |                                                                       |
**      |-----------------------------------------------------------------------+
**      |                              low memory                               |
**      +-----------------------------------------------------------------------+
**   0x10000                                                                 0xA0000    
**                                      (576 ko)
**
**                                VIRTUAL ADDRESS SPACE 
**
**
**
**
**
**                                        PHYSICAL ADDRESS SPACE
**                    
**             0x10000                                                                 0xA0000    
**                +------------------+--------------+-------------------------------------+
**                |  bootloader.bin  |  kernel.bin  |  bootloader usage (page dir/table)  |
**                +------------------+--------------+-------------------------------------+
**        _______/                                                                       /
**       /    __________________________________________________________________________/
**      |   /
**      |--|------------------------------------------------------------------------------------+
**      |                                   low memory                                          |
**      +---------------------------------------------------------------------------------------+
**  0xBFF00000                                                                              0xCFF00000
**                                            (256 mo)
**
**                                      VIRTUAL ADDRESS SPACE 
**
**
**
**
**
**      VGA Display
**      ===========
**
**
**                             PHYSICAL ADDRESS SPACE
**                    
**   0xA0000                                                             0xC0000    
**      +-------------------------------------------------------------------+
**      |                           VGA display                             |
**      +-------------------------------------------------------------------+
**      |                                                                   |
**      |                                                                   |
**      |                                                                   |
**      |-------------------------------------------------------------------|
**      |                           VGA display                             |
**      +-------------------------------------------------------------------+
**   0xA0000                                                             0xC0000    
**                                   (128 ko)
**
**                            VIRTUAL ADDRESS SPACE 
**
**
**
**
**
**      Physical Address (extended)
**      ===========================
**
**
**                              PHYSICAL ADDRESS SPACE
**              
**              (32 pages)                (18 pages)          (4 pages)
**  0x100000                  0x120000                0x132000          0x136000
**      +-------------------------+-----------------------+----------------+
**      |  kpageget() start with  |  kalloc() start with  |  kernel stack  |
**      +-------------------------+-----------------------+----------------+
**       \                       / \                     / \                \__________
**        \___________        __/   \_____________      /   \______________________    |
**                    \      /                    \    |                           \   |
**      +--------------|----|----------------------|---|----------------------------|--|
**      |  low memory  |  kernel pages collection  |  heap ->               <- stack   |
**      +--------------+---------------------------+-----------------------------------+
**  0xBFF00000    0xCFF00000                  0xDFF00000                          0xFFF00000
**          (256 mo)             (256 mo)                        (512 mo)
**
**                              VIRTUAL ADDRESS SPACE 
**
**
*/

#ifndef __ARCH_IA32_ADDR_SPACE_DEF_H__
# define __ARCH_IA32_ADDR_SPACE_DEF_H__


# include <base/types.h>


/* These structures MUST BE same for every architectures.
 * KSEG_COUNT & INITIAL_MAPPING_COUNT values can changed
 * but not their name.
 * This file should NOT be include directly from the kernel,
 * use <common/addr_space_def.h>.
 */
struct _kpage_mapping
{
  paddr_t	paddr;
  vaddr_t	vaddr;
  unsigned	count; /* Number of pages to map */
};

struct _kseg
{
  vaddr_t	base;
  vaddr_t	top;
};

enum _kernel_vaddr_segments {
  LOW_MEMORY_KSEG = 0,
  KPAGE_COLLECTION_KSEG,
  HEAP_STACK_KSEG,
  KSEG_COUNT
};

enum _kernel_initial_mapping {
  LOW_MEMORY_IDENTIFY_MAPPING = 0,
  LOW_MEMORY_MAPPING,
  VGA_IDENTIFY_MAPPING,
  KPAGE_COLLECTION_MAPPING,
  KHEAP_MAPPING,
  KSTACK_MAPPING,
  INITIAL_MAPPING_COUNT
};

struct _addr_space_def
{
  struct _kseg		segments[KSEG_COUNT];
  struct _kpage_mapping	map[INITIAL_MAPPING_COUNT];
};
typedef struct _addr_space_def addr_space_def_t;

addr_space_def_t	default_addr_space_def(void);


#endif /* __ARCH_X86_ADDR_SPACE_DEF_H__ */
