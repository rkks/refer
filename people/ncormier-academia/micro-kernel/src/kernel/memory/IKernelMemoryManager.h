/*
** IKernelMemoryManager.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_KERNEL_MEMORY_MANAGER_H__
# define __KERNEL_MEMORY_I_KERNEL_MEMORY_MANAGER_H__


# include <common/types.h>


/* IKernelMemoryManager Object
 */
typedef struct _iKernelMemoryManager
{
  void*	(*kalloc)	(struct _iKernelMemoryManager* self, size_t size);
  void	(*kfree)	(struct _iKernelMemoryManager* self, void* addr);

  /* Get a physical page and map it in the kernel virtual address space
   */
  void*	(*kpagealloc)	(struct _iKernelMemoryManager* self);
  void	(*kpagefree)	(struct _iKernelMemoryManager* self, void* page_addr);

} __attribute__ ((packed)) * IKernelMemoryManager;

/* Functions
 */
kstatus_t		kernel_memory_manager_setup(void);
IKernelMemoryManager	kernel_memory_manager_singleton(void);


#endif /* __KERNEL_MEMORY_I_KERNEL_MEMORY_MANAGER_H__ */
