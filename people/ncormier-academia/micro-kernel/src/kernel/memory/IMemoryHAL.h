/*
** IMemoryHAL.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_MEMORY_HAL_H__
# define __KERNEL_MEMORY_I_MEMORY_HAL_H__


# include <common/types.h>


/* IHAL Object
 */
typedef struct _iMemoryHAL
{
  /* Virtual mapping helpers
   */
  unsigned unused;
  // map(paddr, vaddr)          => called by hatl
  // unmap(paddr, vaddr)

} __attribute__ ((packed)) * IMemoryHAL;

/* Functions
 */
kstatus_t	memory_hal_setup(void);
IMemoryHAL	memory_hal_singleton(void);


#endif /* __KERNEL_MEMORY_I_MEMORY_HAL_H__ */
