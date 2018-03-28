/*
** IPhysicalPage.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_I_PHYSICAL_PAGE_H__
# define __KERNEL_MEMORY_I_PHYSICAL_PAGE_H__


# include <common/types.h>


typedef struct _iPhysicalPage
{
  void		(*initWithPhysicalAddress)	(struct _iPhysicalPage* self, paddr_t address);
  paddr_t	(*physicalAddress)		(struct _iPhysicalPage* self);

} __attribute__ ((packed)) * IPhysicalPage;


/* Functions
 */
IPhysicalPage	physical_page_alloc(void);


#endif /* __KERNEL_MEMORY_I_PHYSICAL_PAGE_H__ */
