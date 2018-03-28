/*
** MemoryHAL.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY__IMPL_MEMORY_HAL_H__
# define __KERNEL_MEMORY__IMPL_MEMORY_HAL_H__


# include <common/types.h>


/* HAL Object
 */
typedef struct _memoryHAL /* : IMemoryHAL */
{
  /* Virtual mapping helpers
   */
  unsigned unused;
  // map(paddr, vaddr)          => called by hatl
  // unmap(paddr, vaddr)

} __attribute__ ((packed)) * MemoryHAL;


#endif /* __KERNEL_MEMORY__IMPL_MEMORY_HAL_H__ */
