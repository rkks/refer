/*
** main.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/addr_space_def.h>


/* Internal functions forward declaration
 */
static void	_init_gdt(void);
static void	_move_kernel_stack(void* buf, size_t buf_size);
static void	_init_paging(addr_space_def_t* addr_space_def);
static void	_detect_ram(paddr_t* o_base, paddr_t* o_top);
static void	_bootloader_main_stage2(void);

/* Temporary kernel stack
 */
static char	_temp_kern_stack[PAGESIZE];

/* Bootloader C entry point
 * We assume that bootstrap have already :
 *	@ copy in ram the bootloader and the kernel in agreement
 *	  with the address space definition (cf common/addr_space_def.h)
 *	@ setup a valid GDT
 *	@ active the protected mode
 *	@ refresh segments registers
 *	@ setup a stack
 */
void	bootloader_main(void)
{
  /* The bootloader's job is to initialize the kernel virtual machine.
   * After that, it gives the hand to the kernel.
   * Bootloader steps :
   *	@ Setup the segmentation
   *	@ Enable a temporary stack
   *	@ Detect amount of RAM
   *	@ Enable the paging according to the kernel vaddr space definition
   *		(cf common/addr_space_def.h)
   *	@ Setup the real kernel stack
   *	@ Call the kernel
   */

  /* Setup the segmentation
   */
  _init_gdt();

  /* Setup a temporary stack
   */
  _move_kernel_stack(_temp_kern_stack, sizeof(_temp_kern_stack));

  /* Now we have a normal sized stack, continue
   */
  _bootloader_main_stage2();
}

 /* Should be on stack ... 
  * But it's more easy to push statics on stack (for the kernel call)
  */
static paddr_t extended_mem_base, extended_mem_top;

static void	_bootloader_main_stage2(void)
{
  addr_space_def_t addr_space_def = default_addr_space_def();

  /* Detect amount of RAM available
   */
  _detect_ram(&extended_mem_base, &extended_mem_top);

  /* Setup paging according to addr_space_def
   */
  _init_paging(&addr_space_def);

  /* Setup the real kernel stack
   */
  _move_kernel_stack((void*)addr_space_def.map[KSTACK_MAPPING].vaddr,
		     addr_space_def.map[KSTACK_MAPPING].count * PAGESIZE);
  
  /* Jump to kernel
   */
  __asm__ volatile("movl %0, %%eax" : : "r"(extended_mem_top));
  __asm__ volatile("push %eax");
  __asm__ volatile("movl %0, %%eax" : : "r"(extended_mem_base));
  __asm__ volatile("push %eax");
  __asm__ volatile("ljmp $0x8, $0xBFF02000"); /* TODO: SHOULD BE DYNAMIC ! */
}

/* ------ Implementation details ------ */

#include "_gdt.c"
#include "_stack.c"
#include "_ram.c"
#include "_paging.c"
