/*
** PhysicalPage.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __KERNEL_MEMORY_IMPL_PHYSICAL_PAGE_H__
# define __KERNEL_MEMORY_IMPL_PHYSICAL_PAGE_H__


# include <memory/IPhysicalPage.h>
# include <common/types.h>


/* PhysicalPage Object
 */
typedef struct _physicalPage /* : IPhysicalPage */
{
  void		(*initWithPhysicalAddress)	(struct _physicalPage* self, paddr_t address);
  paddr_t	(*physicalAddress)		(struct _physicalPage* self);

  /* Private members
   */
  paddr_t	_physicalAddress;

} __attribute__ ((packed)) * PhysicalPage;


#endif /* __KERNEL_MEMORY_IMPL_PHYSICAL_PAGE_H__ */
