/*
** kernel-dependant.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#define __KERNEL_DEPENDANT_C__
#include "kernel-dependant.h"

void	rfree(void *addr, struct malloc_type *type)
{
  FREE(addr, type);
}

void*	rmalloc(unsigned long size, struct malloc_type *type, int flags)
{
  return malloc(size, type, flags);
}

MALLOC_DEFINE(M_FILETRIGGER, "file-trigger-allocation", "file trigger allocation");
