/*
** server.c for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <strings.h>
# include <stdio.h>
# define  EXCMD_LOCK()
# define  EXCMD_UNLOCK()
#else
# include <sys/libkern.h>
# include "kernel-dependant.h"
# define  EXCMD_LOCK()	  mtx_lock(&(self->lock));
# define  EXCMD_UNLOCK()  mtx_unlock(&(self->lock));
#endif /* _KERNEL */
#include "server.h"
#include "elem.h"
#include "host.h"
#include "socket.h"
#include "die.h"
#include "buffer.h"

/* Forward declaration
 */
/* Methods
 */
static int		init_with_port(server_t* self, short port);
static void		destroy(server_t* self);
static void		loop(server_t* self);
static elem_t*		get_elem_by_id(server_t* self, unsigned id);
static dist_elem_t*	get_dist_elem_by_id(server_t* self, unsigned id);
static void		on_extern_command(server_t* self, struct lst_excmd* excmd);
static int		on_create_request(server_t* self, unsigned size, host_t* src);
static void		on_create_answer(server_t* self, unsigned id, host_t* from);
static void		on_update_finished(server_t* self, unsigned id);
/* Others
 */
static host_t*		host_manager(server_t* self, char* addr, short port);
static void		insert_new_host(server_t* self, host_t* host);
#if 0 /* UNUSED */
static void		insert_new_elem(server_t* self, elem_t* elem);
#endif
static void		insert_new_dist_elem(server_t* self, dist_elem_t* elem);

/* Extern functions
 */
server_t*	get_server(void)
{
  static server_t* singleton = NULL;
  if (!singleton)
    {
      server_t* serv = malloc(sizeof(server_t));
      if (!serv) die("get_server: server allocation failed");
      bzero(serv, sizeof(server_t));
      /* Init methods
       */
      serv->init_with_port = init_with_port;
      serv->destroy = destroy;
      serv->loop = loop;
      serv->get_elem_by_id = get_elem_by_id;
      serv->get_dist_elem_by_id = get_dist_elem_by_id;
      serv->on_extern_command = on_extern_command;
      serv->on_create_request = on_create_request;
      serv->on_create_answer = on_create_answer;
      serv->on_update_finished = on_update_finished;
      singleton = serv;
    }
  return singleton;
}

/* Local functions
 */
static int	init_with_port(server_t* self, short port)
{
  /* Init list
   */
  LIST_INIT(&self->elems);
  LIST_INIT(&self->dist_elems);
  LIST_INIT(&self->hosts);
  LIST_INIT(&self->extern_commands);
  TAILQ_INIT(&self->create_tasks);
  LIST_INIT(&self->update_tasks);
#ifdef _KERNEL
  /* Init lock
   */
  mtx_init(&(self->lock), "netmalloc-serv", NULL, MTX_SPIN);
#endif /* _KERNEL */
  /* Open listening port
   */
  self->listening_socket = alloc_socket();
  if (!self->listening_socket)
    return -1;
  int ret = self->listening_socket->init_listening_socket_on_port(self->listening_socket, port);
  if (!ret) self->initialized = 1;
  return ret;
}

static void	destroy(server_t* self)
{
  /* Cleanup
   */
  struct lst_elem* lst_elem = NULL, * lst_elem_tmp = NULL;
  LIST_FOREACH_SAFE(lst_elem, &self->elems, elem_list, lst_elem_tmp)
    {
      elem_t* elem = lst_elem->elem;
      elem->destroy(elem);
      LIST_REMOVE(lst_elem, elem_list);
      free(lst_elem);
    }
  struct lst_dist_elem* lst_dist_elem = NULL, * lst_dist_elem_tmp = NULL;
  LIST_FOREACH_SAFE(lst_dist_elem, &self->dist_elems, dist_elem_list, lst_dist_elem_tmp)
    {
      dist_elem_t* dist_elem = lst_dist_elem->elem;
      dist_elem->destroy(dist_elem);
      LIST_REMOVE(lst_dist_elem, dist_elem_list);
      free(lst_dist_elem);
    }
  struct lst_host* lst_host = NULL, * lst_host_tmp = NULL;
  LIST_FOREACH_SAFE(lst_host, &self->hosts, host_list, lst_host_tmp)
    {
      host_t* host = lst_host->host;
      host->destroy(host);
      LIST_REMOVE(lst_host, host_list);
      free(lst_host);
    }
  /* TODO: delete extern_commands update_tasks, and create_tasks
   */
  if (self->listening_socket)
    (void) self->listening_socket->destroy(self->listening_socket);
  free(self);
}

static void	loop(server_t* self)
{
  int count = 0;
#ifdef _KERNEL
  void* waddr = &count; /* Dummy assign for tsleep */
#endif /* _KERNEL */
  while (1)
    {
      /*
       * 1) check listening socket
       *   - if new client -> new host -> add to hosts
       * 2) check read
       *   - get:id:offset -> foreach elems; elem.id == id; elem.send_contents()
       *   - contents:id:offset -> foreach dist_elems; dist_elem.id == id; dist_elem.update_contents()
       * 3) check write
       *   - foreach hosts; if host.output_fifo; host.send()
       */
      /* Check external commands
       */
      struct lst_excmd* lst_excmd = NULL, * lst_excmd_tmp = NULL;
      LIST_FOREACH_SAFE(lst_excmd, &self->extern_commands, excmd_list, lst_excmd_tmp)
	{
	  /* New external command
	   */
	  self->on_extern_command(self, lst_excmd);
	  LIST_REMOVE(lst_excmd, excmd_list);
	  free(lst_excmd);
	}
      /* Check new host
       */
      socket_t* sock = self->listening_socket->accept(self->listening_socket);
      if (sock)
	{
	  /* New host
	   */
	  host_t* new_host = alloc_host();
	  if (!new_host) die("loop: host allocation failed");
	  new_host->init_with_socket(new_host, sock);
	  insert_new_host(self, new_host);
	}
      /* Check hosts sockets
       */
      struct lst_host* lst_host = NULL;
      LIST_FOREACH(lst_host, &self->hosts, host_list)
	{
	  host_t* host = lst_host->host;
	  if (host->socket->has_something_to_read(host->socket) > 0)  /* Something to read ? */
	    host->recv(host);
	  if (LIST_FIRST(&host->output_fifo)) /* Something to write ? */
	    host->send(host);	  
	  /* Check host status 
	   */
	  /* TODO */
	}
#ifndef _KERNEL
      usleep(1000);
#else
      (void) tsleep(waddr, PCATCH|PWAIT, "idle", 100);
#endif /* _KERNEL */
    }
}

static elem_t*	get_elem_by_id(server_t* self, unsigned id)
{
  struct lst_elem* lst_elem = NULL;
  LIST_FOREACH(lst_elem, &self->elems, elem_list)
    {
      elem_t* elem = lst_elem->elem;
      if (elem->id == id)
	return elem;
    }
  return NULL;
}

static dist_elem_t*	get_dist_elem_by_id(server_t* self, unsigned id)
{
  struct lst_dist_elem* lst_dist_elem = NULL;
  LIST_FOREACH(lst_dist_elem, &self->dist_elems, dist_elem_list)
    {
      dist_elem_t* dist_elem = lst_dist_elem->elem;
      if (dist_elem->id == id)
	return dist_elem;
    }
  return NULL;
}

static void		on_extern_command(server_t* self, struct lst_excmd* excmd)
{
  switch (excmd->command)
    {
    case NETMALLOC:
      {
	/* Check host
	 */
	host_t* src = host_manager(self, excmd->data.netmalloc_args.host, excmd->data.netmalloc_args.port);
	if (!src) return; /* Host does not respond ? */
	/* Insert new create task
	 */
	struct lst_create_tsk* new_ctsk = malloc(sizeof(struct lst_create_tsk));
	if (!new_ctsk) die("on_extern_command: tsk allocation failed");
	new_ctsk->waddr = excmd->data.netmalloc_args.waddr; /* Wakeup addr */
	new_ctsk->kvaddr = excmd->data.netmalloc_args.kvaddr; /* Kernel buffer virtual address */
	new_ctsk->host = src;
	new_ctsk->size = excmd->data.netmalloc_args.size; /* Requested size */
	new_ctsk->id = excmd->data.netmalloc_args.id;
	TAILQ_INSERT_TAIL(&self->create_tasks, new_ctsk, create_tsk_list); /* Insert in queue */
	/* Query allocation to host
	 */
	src->query_allocation(src, excmd->data.netmalloc_args.size); /* Send request */
      }
      break;
    case NETFREE:
      break;
    case NETATTACH:
      {
	/* Check host
	 */
	host_t* src = host_manager(self, excmd->data.netattach_args.host, excmd->data.netattach_args.port);
	if (!src) return; /* Host does not respond ? */
	/* Create dist_elem
	 */
	dist_elem_t* new_delem = alloc_dist_elem();
	if (!new_delem) die("on_extern_command: dist_elem allocation failed");
	new_delem->init_with_id_size_and_host(new_delem, excmd->data.netattach_args.id, excmd->data.netattach_args.size, src);
	insert_new_dist_elem(self, new_delem);	
	/* Query update to host
	 */
	src->query_update(src, excmd->data.netattach_args.id); /* Send request */
	/* Insert new update task
	 */
	struct lst_update_tsk* new_utsk = malloc(sizeof(struct lst_update_tsk));
	if (!new_utsk) die("on_extern_command: tsk allocation failed");
	new_utsk->waddr = excmd->data.netattach_args.waddr; /* Wakeup addr */
	new_utsk->kvaddr = excmd->data.netattach_args.kvaddr; /* Kernel buffer virtual address */
	new_utsk->id = excmd->data.netattach_args.id; /* Requested id */
	LIST_INSERT_HEAD(&self->update_tasks, new_utsk, update_tsk_list); /* Insert in queue */
      }
      break;
    case NETDETACH:
      {
	unsigned id = excmd->data.netdetach_args.id;
	dist_elem_t* elem = self->get_dist_elem_by_id(self, id);
	if (!elem) die("on_extern_command: distant elem not found");
	/* Send update to host
	 */
	elem_t* local_elem = elem->local_elem;
	local_elem->send_contents(local_elem, 0, elem->src);
	/* Insert new update task
	 */
	struct lst_update_tsk* new_utsk = malloc(sizeof(struct lst_update_tsk));
	if (!new_utsk) die("on_extern_command: tsk allocation failed");
	new_utsk->waddr = excmd->data.netattach_args.waddr; /* Wakeup addr */
	new_utsk->kvaddr = excmd->data.netattach_args.kvaddr; /* Kernel buffer virtual address */
	new_utsk->id = excmd->data.netattach_args.id; /* Requested id */
	LIST_INSERT_HEAD(&self->update_tasks, new_utsk, update_tsk_list); /* Insert in queue */
      }
      break;
    case NETGETID:
      break;
    default:
      ; /* die("Unknown command"); */
    }
}

static int		on_create_request(server_t* self, unsigned size, host_t* src)
{
  dist_elem_t* new_elem = alloc_dist_elem();
  if (!new_elem) die("on_create_request: dist_elem allocation failed");
  new_elem->init_with_id_size_and_host(new_elem, (unsigned)-1/* UNDEF */, size, src);
  insert_new_dist_elem(self, new_elem);  
  return new_elem->id;
}

static void		on_create_answer(server_t* self, unsigned id, host_t* from)
{
  struct lst_create_tsk* tsk = NULL;
  TAILQ_FOREACH(tsk, &self->create_tasks, create_tsk_list)
    {
      if (from == tsk->host)
	{
	  /* Create dist_elem
	   */
	  dist_elem_t* new_delem = alloc_dist_elem();
	  if (!new_delem) die("on_create_answer: dist_elem allocation failed");
	  new_delem->init_with_id_size_and_host(new_delem, id, tsk->size, tsk->host);
	  insert_new_dist_elem(self, new_delem);
	  /* Assign kvaddr & id
	   */
	  *tsk->kvaddr = (void*)new_delem->local_elem->buffer->buf;
	  *tsk->id = id;
#ifdef _KERNEL
	  /* Wakeup user process
	   */
	  wakeup(tsk->waddr);
#else
	  /* Wakeup thread
	   */	  
	  *((int*)tsk->waddr) = 1;
#endif /* _KERNEL */
	  printf("Distant allocation succeed with id %d on %x\n", id, (unsigned)new_delem->local_elem->buffer->buf);
	  TAILQ_REMOVE(&self->create_tasks, tsk, create_tsk_list);
	  return;
	}
    }
  die("on_create_answer: host not found");
}

static void		on_update_finished(server_t* self, unsigned id)
{
  printf("Update finished on %d (%s)\n", id, get_dist_elem_by_id(self, id)->local_elem->buffer->buf);
  struct lst_update_tsk* tsk = NULL;
  LIST_FOREACH(tsk, &self->update_tasks, update_tsk_list)
    {
      if (tsk->id == id)
	{
	  dist_elem_t* delem = get_dist_elem_by_id(self, id);
	  if (delem)
	    {
	      /* Assign kvaddr
	       */
	      *tsk->kvaddr = (void*)delem->local_elem->buffer->buf;
#ifdef _KERNEL
	      /* Wakeup user process
	       */
	      wakeup(tsk->waddr);
#else
	      /* Wakeup thread
	       */	  
	      *((int*)tsk->waddr) = 1;
#endif /* _KERNEL */
	    }
	  LIST_REMOVE(tsk, update_tsk_list);
	  return;
	}
    }
}

/* Host helper
 */
static host_t*		host_manager(server_t* self, char* addr, short port)
{
  /* Try to find host
   */
  host_t* src = NULL;
  struct lst_host* lst_host = NULL;
  LIST_FOREACH(lst_host, &self->hosts, host_list)
    {
      host_t* host = lst_host->host;
      if (host->addr && !strcmp(addr, host->addr) && host->port == port)
	{
	  src = host;
	  break;
	}
    }
  if (!src) /* Host not found, try to create it */
    {
      src = alloc_host();
      if (src->init_with_addr(src, addr, port)) return NULL; /* die("synchronize: init_with_addr failed"); */
      insert_new_host(self, src);
    }
  return src;
}

/* List helpers
 */
static void	insert_new_host(server_t* self, host_t* host)
{
  struct lst_host* entry = malloc(sizeof(struct lst_host));
  if (!entry) die("insert_new_host: lst_host allocation failed");
  entry->host = host;
  LIST_INSERT_HEAD(&self->hosts, entry, host_list);	  
}

#if 0 /* UNUSED */
static void	insert_new_elem(server_t* self, elem_t* elem)
{
  struct lst_elem* entry = malloc(sizeof(struct lst_elem));
  if (!entry) die("insert_new_elem: lst_elem allocation failed");
  entry->elem = elem;
  LIST_INSERT_HEAD(&self->elems, entry, elem_list);	  
}
#endif

static void	insert_new_dist_elem(server_t* self, dist_elem_t* elem)
{
  struct lst_dist_elem* entry = malloc(sizeof(struct lst_dist_elem));
  if (!entry) die("insert_new_dist_elem: lst_dist_elem allocation failed");
  entry->elem = elem;
  LIST_INSERT_HEAD(&self->dist_elems, entry, dist_elem_list);	  
}
