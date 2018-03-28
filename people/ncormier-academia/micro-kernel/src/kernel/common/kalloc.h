/*
** kalloc.h for  in /data/work/kernel/mkernel/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Sun Dec  2 19:53:08 2007 nicolas
** Last update Sun Dec  9 17:43:53 2007 nicolas
*/

#ifndef __KERNEL_COMMON_K_ALLOC_H__
# define __KERNEL_COMMON_K_ALLOC_H__


# include <common/types.h>
# include <memory/IKernelMemoryManager.h>


static inline void*	kalloc(size_t size)
{
  IKernelMemoryManager is = kernel_memory_manager_singleton();
  return is->kalloc(is, size);
}

static inline void	kfree(void* tofree)
{
  IKernelMemoryManager is = kernel_memory_manager_singleton();
  is->kfree(is, tofree);
}


#endif /* __KERNEL_COMMON_K_ALLOC_H__ */
