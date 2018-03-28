/*
** netmalloc_common.h for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#ifndef __NETMALLOC_COMMON_H__
# define __NETMALLOC_COMMON_H__

/* Public types definition
 */
struct netmalloc_args
{
  enum
    {
      DO_NETMALLOC  = 0,
      DO_NETATTACH,
      DO_NETDETACH,
      DO_NETREAD,
      DO_NETWRITE
    }		action;
  void*		vaddr; /* Input */
  unsigned	id; /* Input */
  unsigned	size; /* Input */
  void**	o_vaddr; /* Output */
  unsigned*	o_id; /* Output */
  char*		iobuf;
  unsigned	iosize;
};

#ifdef _KERNEL
/* Public function declaration
 */
int	netmalloc_action(struct thread* th, struct netmalloc_args* args, void** kaddr);
#endif /* _KERNEL */

#endif /* __NETMALLOC_COMMON_H__ */
