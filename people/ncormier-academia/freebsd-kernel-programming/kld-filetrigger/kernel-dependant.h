/*
** kernel-dependant.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __KERNEL_DEPENDANT_H__
# define __KERNEL_DEPENDANT_H__

/* Extra declaration
 */
# ifdef _KERNEL

#  include <sys/types.h>
#  include <sys/param.h>
#  include <sys/malloc.h>
#  include <sys/kernel.h>

void	rfree(void *addr, struct malloc_type *type);
void*	rmalloc(unsigned long size, struct malloc_type *type, int flags);

#ifndef __KERNEL_DEPENDANT_C__
#  define malloc(s)	malloc(s, M_FILETRIGGER, M_WAITOK)
#  define free(p)	free(p, M_FILETRIGGER)
#endif /* __KERNEL_DEPENDANT_C__ */
MALLOC_DECLARE(M_FILETRIGGER);

# define strdup(s)	strdup(s, M_FILETRIGGER)

# endif /* _KERNEL */


#endif /* __KERNEL_DEPENDANT_H__ */
