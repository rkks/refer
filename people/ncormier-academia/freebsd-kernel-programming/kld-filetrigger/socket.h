/*
** socket.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __SOCKET_H__
# define __SOCKET_H__

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
socket_t*	alloc_socket(void);

/* Private typedef
 */
#ifndef _KERNEL
struct pv
{
  int	sock;
};
#else
struct pv
{
  struct socket* sock;
};
#endif /* _KERNEL */

/* File struct
 */
struct msocket
{
  /* Properties
   */
  struct pv*	pvdata; /* Abstract data */
  /* Methods
   */
  int				(*init_listening_socket_on_port)(socket_t* self, unsigned port);
  int				(*init_and_connect_to)(socket_t* self, const char* addr, unsigned port);
  void				(*destroy)(socket_t* self);
  socket_t*			(*accept)(socket_t* self);
  int				(*has_something_to_read)(socket_t* self);
  int				(*read)(socket_t* self, char* buf, unsigned size);
  int				(*write)(socket_t* self, char* buf, unsigned size);
};

#endif /* __FILE_H__ */
