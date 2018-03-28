/*
** PhysicalPage.c
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#include <common/kalloc.h>
#include "PhysicalPage.h"


/* Forward declaration
 */
static void	_initWithPhysicalAddress(struct _physicalPage* self, paddr_t address);
static paddr_t	_physicalAddress(struct _physicalPage* self);

/* Implementation
 */
IPhysicalPage	physical_page_alloc(void)
{
  PhysicalPage ret = kalloc(sizeof(struct _physicalPage));

  /* Init members
   */
  ret->initWithPhysicalAddress = _initWithPhysicalAddress;
  ret->physicalAddress = _physicalAddress;

  return (IPhysicalPage) ret;
}

static void	_initWithPhysicalAddress(struct _physicalPage* self, paddr_t address)
{
  self->_physicalAddress = address;
}

static paddr_t	_physicalAddress(struct _physicalPage* self)
{
  return self->_physicalAddress;
}
