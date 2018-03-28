/*
** server.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __SERVER_H__
# define __SERVER_H__

# include <sys/queue.h>
# ifdef _KERNEL
#  include <sys/param.h>
#  include <sys/lock.h>
#  include <sys/mutex.h>
# endif /* _KERNEL */

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
server_t*	get_server(void);
dist_elem_t*	server_get_dist_elem_by_id(unsigned id);
elem_t*		server_get_elem_by_id(unsigned id);
elem_t*		server_get_elem_by_path(char *path);

/* Private typedef
 */
struct lst_elem
{
  elem_t*			elem;
  LIST_ENTRY(lst_elem)		elem_list;
};

struct lst_dist_elem
{
  dist_elem_t*			elem;
  LIST_ENTRY(lst_dist_elem)	dist_elem_list;
};

struct lst_host
{
  host_t*		host;
  LIST_ENTRY(lst_host)	host_list;
};

struct lst_excmd	/* External command to drive the daemon */
{
  enum {
    SHARE = 0,
    REGISTER,
    TIMEOUT,
    ENCRYPT
  }					command;
  union
  {
    struct
    {
      char*	path;
    }			share_arg;
    struct
    {
      char*	addr;
      short	port;
      unsigned	id;
      char*	path;
    }			register_arg;
    struct
    {
      unsigned	timeo;
    }			timeout_arg;
    struct
    {

    }			encrypt_arg;
  }					data;
  LIST_ENTRY(lst_excmd)			excmd_list;
};

/* Server struct
 */
struct server
{
  /*  xxx timeout; */
  int				initialized;
  socket_t*			listening_socket;
  LIST_HEAD(, lst_elem)		elems;
  LIST_HEAD(, lst_dist_elem)	dist_elems;
  LIST_HEAD(, lst_host)		hosts;
#ifdef _KERNEL
  struct mtx			lock; /* Lock for extern_commands */
#endif /* _KERNEL */
  LIST_HEAD(, lst_excmd)	extern_commands; /* External command list, protected by a mutex */
  /* Methods
   */
  /* Deamon methods
   */
  int				(*init_with_port)(server_t* self, short port);
  void				(*destroy)(server_t* self);
  void				(*loop)(server_t* self);
  elem_t*			(*get_elem_by_id)(server_t* self, unsigned id);
  elem_t*			(*get_elem_by_path)(server_t* self, char* path);
  dist_elem_t*			(*get_dist_elem_by_id)(server_t* self, unsigned id);
  int				(*synchronize)(server_t* self, char* src_addr, short src_port, unsigned dist_id, char* path);
  /* User methods (Thread-safe)
   */
  void				(*command_share)(server_t* self, char* path);
  void				(*command_register)(server_t* self, char* addr, short port, unsigned id, char* path);
};

#endif /* __SERVER_H__ */
