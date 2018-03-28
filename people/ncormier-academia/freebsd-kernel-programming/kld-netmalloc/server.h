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

struct lst_create_tsk
{
  void*				waddr;	/* Wakeup address */
  void**			kvaddr; /* Kernel buffer virtual address */
  unsigned*			id;
  host_t*			host;
  unsigned			size;
  TAILQ_ENTRY(lst_create_tsk)	create_tsk_list;
};

struct lst_update_tsk
{
  void*				waddr;	/* Wakeup address */
  void**			kvaddr; /* Kernel buffer virtual address */
  unsigned			id;	/* Waiting for */
  unsigned			size;	/* Requested size */
  LIST_ENTRY(lst_update_tsk)	update_tsk_list;
};

struct lst_excmd	/* External command to drive the daemon */
{
  enum {
    NETMALLOC = 0,
    NETFREE,
    NETATTACH,
    NETDETACH,
    NETGETID
  }				command;
  union
  {
    struct
    {
      unsigned	size;
      char*	host;
      short	port;
      void*	waddr;	/* Wakeup address */
      void**	kvaddr; /* Kernel buffer virtual address */
      unsigned*	id; /* Ressource id */
    }		netmalloc_args;
    struct
    {
      unsigned	id;
    }		netfree_args;
    struct
    {
      unsigned	id;
      unsigned	size;
      char*	host;
      short	port;
      void*	waddr;	/* Wakeup address */
      void**	kvaddr; /* Kernel buffer virtual address */
    }		netattach_args;
    struct
    {
      unsigned	id; /* void*	vaddr; */
      void*	waddr;	/* Wakeup address */
    }		netdetach_args;
    struct
    {
      void*	vaddr;
    }		netgetid_args;
  }				data;
  LIST_ENTRY(lst_excmd)		excmd_list;
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
  TAILQ_HEAD(, lst_create_tsk)	create_tasks; /* Users waiting for distant buffer creation */
  LIST_HEAD(, lst_update_tsk)	update_tasks; /* Users waiting for local buffer synchonization */
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
  dist_elem_t*			(*get_dist_elem_by_id)(server_t* self, unsigned id);
  void				(*on_extern_command)(server_t* self, struct lst_excmd* excmd);
  int				(*on_create_request)(server_t* self, unsigned size, host_t* src);
  void				(*on_create_answer)(server_t* self, unsigned id, host_t* from);
  void				(*on_update_finished)(server_t* self, unsigned id);
};

#endif /* __SERVER_H__ */
