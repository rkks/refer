/*
** MemoryHAL.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <memory/IMemoryHAL.h>
#include <base/debug.h>
#include "MemoryHAL.h"


/* Forward declaration
 */
kstatus_t	IA32_memory_hal_setup(void);

/* Object allocation
 */
static struct _memoryHAL	_memoryHAL_singleton;

/* Singleton
 */
IMemoryHAL	memory_hal_singleton(void)
{
  return (IMemoryHAL)&_memoryHAL_singleton;
}

/* Setup function
 */
kstatus_t	memory_hal_setup(void)
{
  /* Init singleton
   */
  //_memoryHAL_singleton.extendedMemBase = extended_mem_base;

  return KS_SUCCESS;;
}
