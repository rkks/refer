/*
** _gdt.c
** GDT Setup for IA32
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <arch/x86/x86_segmentation_types.h>


/* The GDT
 */
enum	_gdt_desc {

  NULL_DESC = 0,
  KERNEL_CODE_DESC,
  KERNEL_DATA_DESC,
  GDT_SEG_NUM
};

static x86_gdte_t	_the_gdt[GDT_SEG_NUM];

static void	_init_gdt(void)
{
  /* Null descriptor
   */
  _the_gdt[NULL_DESC].value.low = 0;
  _the_gdt[NULL_DESC].value.high = 0;

  /* Kernel code descriptor
   */
  _the_gdt[KERNEL_CODE_DESC].prop.base_1 = 0;
  _the_gdt[KERNEL_CODE_DESC].prop.base_2 = 0;
  _the_gdt[KERNEL_CODE_DESC].prop.base_3 = 0;
  _the_gdt[KERNEL_CODE_DESC].prop.limit_1 = ~0;
  _the_gdt[KERNEL_CODE_DESC].prop.limit_2 = ~0;
  _the_gdt[KERNEL_CODE_DESC].prop.type = CODE_READ_EXEC;
  _the_gdt[KERNEL_CODE_DESC].prop.s = CODE_OR_DATA_SEGMENT;
  _the_gdt[KERNEL_CODE_DESC].prop.dpl = RING_0;
  _the_gdt[KERNEL_CODE_DESC].prop.p = SEGMENT_IS_PRESENT;
  _the_gdt[KERNEL_CODE_DESC].prop.db = SEGMENT_32BITS;
  _the_gdt[KERNEL_CODE_DESC].prop.g = PAGE_GRANULARITY;

  /* Kernel code descriptor
   */
  _the_gdt[KERNEL_DATA_DESC].prop.base_1 = 0;
  _the_gdt[KERNEL_DATA_DESC].prop.base_2 = 0;
  _the_gdt[KERNEL_DATA_DESC].prop.base_3 = 0;
  _the_gdt[KERNEL_DATA_DESC].prop.limit_1 = ~0;
  _the_gdt[KERNEL_DATA_DESC].prop.limit_2 = ~0;
  _the_gdt[KERNEL_DATA_DESC].prop.type = DATA_READ_WRITE;
  _the_gdt[KERNEL_DATA_DESC].prop.s = CODE_OR_DATA_SEGMENT;
  _the_gdt[KERNEL_DATA_DESC].prop.dpl = RING_0;
  _the_gdt[KERNEL_DATA_DESC].prop.p = SEGMENT_IS_PRESENT;
  _the_gdt[KERNEL_DATA_DESC].prop.db = SEGMENT_32BITS;
  _the_gdt[KERNEL_DATA_DESC].prop.g = PAGE_GRANULARITY;

  /* GDTR
   */
  x86_gdtr_t gdtr;
  gdtr.size = sizeof(_the_gdt) - 1;
  gdtr.addr = (unsigned long) _the_gdt;

  /* Apply GDT
   */
  __asm__ volatile("lgdt %0" : : "m" (gdtr));

  /* Refresh registers
   */
  __asm__ volatile("ljmp %0,$_init_gdtr_jmp" :  : "i" (BUILD_SEGMENT_REG_VALUE(KERNEL_CODE_DESC)));
  __asm__ volatile("_init_gdtr_jmp:");
  __asm__ volatile("mov %0, %%ax": : "i" (BUILD_SEGMENT_REG_VALUE(KERNEL_DATA_DESC)));
  __asm__ volatile("mov %ax, %ds");
  __asm__ volatile("mov %ax, %ss");
  __asm__ volatile("mov %ax, %es");
  __asm__ volatile("mov %ax, %fs");
  __asm__ volatile("mov %ax, %gs");
}
