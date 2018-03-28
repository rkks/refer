/*
** IVirtualMemoryPage.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_VIRTUAL_MEMORY_PAGE_H__
# define __KERNEL_MEMORY_I_VIRTUAL_MEMORY_PAGE_H__


# include <common/types.h>
# include <memory/IVirtualMemorySegment.h>
# include <memory/IMemoryPage.h>


/* IVirtualMemoryPage Object
 */
typedef struct _iVirtualMemoryPage
{
  void			(*init)		(struct _iVirtualMemoryPage* self, IMemoryPage memory_page, IVirtualMemorySegment memory_seg);
  IVirtualMemorySegment	(*segment)	(struct _iVirtualMemoryPage* self);
  IMemoryPage		(*memoryPage)	(struct _iVirtualMemoryPage* self);

} __attribute__ ((packed)) * IVirtualMemoryPage;


#endif /* __KERNEL_MEMORY_I_VIRTUAL_MEMORY_PAGE_H__ */
