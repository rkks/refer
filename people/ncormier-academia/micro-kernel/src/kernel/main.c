/*
** main.c for  in /data/work/kernel/micro-kernel/src/kernel
** 
** Made by nicolas
*/

#include <base/debug.h>
#include <memory/IMemoryHAL.h>
#include <memory/IKernelMemoryManager.h>
#include <memory/IPhysicalMemoryManager.h>


static void	sleep(void)
{
  for (unsigned j = 0; j < 0xFFFFFF; j++);
}

/* Kernel C entry point
 * We assume that the bootloader has already setup
 * the virtual machine according to the kernel virtual
 * address space definition (cf common/addr_space_def.h)
 */
void	kernel_main(paddr_t extended_mem_base, paddr_t extended_mem_top)
{
  console_clear();

  TRACE("Micro-kernel entry point");
  TRACE("------------------------");
  TRACE("");

  kstatus_t status = KS_FAILED;

  /* Initialize memory HAL singleton
   */
  TRACE(" memory_hal_setup()");
  status = memory_hal_setup();

  /* Initialize kernel memory manager singleton
   */
  TRACE(" kernel_memory_manager_setup()");
  status = kernel_memory_manager_setup();


  /* Initialize physical memory manager singleton
   */
  TRACE(" physical_memory_manager_setup()");
  status = physical_memory_manager_setup(extended_mem_base, extended_mem_top);
  

  while (1)
    {
      sleep();
      console_write(".");
    }
}
