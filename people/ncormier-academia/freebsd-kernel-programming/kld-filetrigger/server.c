/*
** server.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <unistd.h>
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

/* Forward declaration
 */
static int		init_with_port(server_t* self, short port);
static void		destroy(server_t* self);
static void		loop(server_t* self);
static elem_t*		get_elem_by_id(server_t* self, unsigned id);
static elem_t*		get_elem_by_path(server_t* self, char* path);
static dist_elem_t*	get_dist_elem_by_id(server_t* self, unsigned id);
static int		share(server_t* self, char* path);
static int		synchronize(server_t* self, char* src_addr, short src_port, unsigned dist_id, char* path);
static int		server_timeout(server_t* self, unsigned timo);
static void		command_share(server_t* serv, char* path);
static void		command_register(server_t* serv, char* addr, short port, unsigned id, char* path);

/* Extern functions
 */
server_t*	get_server(void)
{
  static server_t* singleton = NULL;
  if (!singleton)
    {
      server_t* serv = malloc(sizeof(server_t));
      if (!serv)
	return NULL;
      bzero(serv, sizeof(server_t));
      /* Init methods
       */
      serv->init_with_port = init_with_port;
      serv->destroy = destroy;
      serv->loop = loop;
      serv->get_elem_by_id = get_elem_by_id;
      serv->get_elem_by_path = get_elem_by_path;
      serv->get_dist_elem_by_id = get_dist_elem_by_id;
      serv->synchronize = synchronize;
      serv->command_share = command_share;
      serv->command_register = command_register;
      singleton = serv;
    }
  return singleton;
}

dist_elem_t*	server_get_dist_elem_by_id(unsigned id)
{
  server_t* serv = get_server();
  if (!serv) die("server_get_dist_elem_by_id: can't get current server");
  return serv->get_dist_elem_by_id(serv, id);
}

elem_t*		server_get_elem_by_id(unsigned id)
{
  server_t* serv = get_server();
  if (!serv) die("server_get_elem_by_id: can't get current server");
  return serv->get_elem_by_id(serv, id);
}

elem_t*		server_get_elem_by_path(char *path)
{
  server_t* serv = get_server();
  if (!serv) die("server_get_elem_by_path: can't get current server");
  return serv->get_elem_by_path(serv, path);
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
#ifdef _KERNEL
  /* Init lock
   */
  mtx_init(&(self->lock), "ftrigger-serv", NULL, MTX_SPIN);
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
  if (self->listening_socket)
    (void) self->listening_socket->destroy(self->listening_socket);
  free(self);
}

static void	insert_new_host(server_t* self, host_t* host)
{
  struct lst_host* entry = malloc(sizeof(struct lst_host));
  if (!entry) die("insert_new_host: lst_host allocation failed");
  entry->host = host;
  LIST_INSERT_HEAD(&self->hosts, entry, host_list);	  
}

static void	insert_new_elem(server_t* self, elem_t* elem)
{
  struct lst_elem* entry = malloc(sizeof(struct lst_elem));
  if (!entry) die("insert_new_elem: lst_elem allocation failed");
  entry->elem = elem;
  LIST_INSERT_HEAD(&self->elems, entry, elem_list);	  
}

static void	insert_new_dist_elem(server_t* self, dist_elem_t* elem)
{
  struct lst_dist_elem* entry = malloc(sizeof(struct lst_dist_elem));
  if (!entry) die("insert_new_dist_elem: lst_dist_elem allocation failed");
  entry->elem = elem;
  LIST_INSERT_HEAD(&self->dist_elems, entry, dist_elem_list);	  
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
	  switch (lst_excmd->command)
	    {
	    case SHARE:
	      if (share(self, lst_excmd->data.share_arg.path))
		; /* TODO: errorprint */
	      free(lst_excmd->data.share_arg.path);
	      break;
	    case REGISTER:
	      if (synchronize(self, lst_excmd->data.register_arg.addr, lst_excmd->data.register_arg.port, 
			      lst_excmd->data.register_arg.id, lst_excmd->data.register_arg.path))
		; /* TODO: errorprint */
	      free(lst_excmd->data.register_arg.addr);
	      free(lst_excmd->data.register_arg.path);
	      break;
	    case TIMEOUT:
	      if (server_timeout(self, 0))
		; /* TODO: errorprint */
	    default:
	      ; /* die("Unknown command"); */
	    }
	  LIST_REMOVE(lst_excmd, excmd_list);
	  free(lst_excmd);
	}
      /* Check new host
       */
      socket_t* sock = self->listening_socket->accept(self->listening_socket);
      if (sock)
	{
	  /* New host !
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
	  if (host->socket->has_something_to_read(host->socket) > 0)
	    host->recv(host);
	  if (LIST_FIRST(&host->output_fifo))
	    host->send(host);	  
	  /* Check host status 
	   */
	}
      if (++count == 100) /* Timeout */
	{
	  printf("a virer\n");
	  count = 0;
	  /* Reload all distant elems
	   */
	  struct lst_dist_elem* lst_dist_elem = NULL;
	  LIST_FOREACH(lst_dist_elem, &self->dist_elems, dist_elem_list)
	    {
	      dist_elem_t* dist_elem = lst_dist_elem->elem;
	      dist_elem->query_update(dist_elem);
	    }
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

static elem_t*		get_elem_by_path(server_t* self, char* path)
{ 
  struct lst_elem* lst_elem = NULL;
  LIST_FOREACH(lst_elem, &self->elems, elem_list)
    {
      elem_t* elem = lst_elem->elem;
      if (!strcmp(elem->path, path))
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

/* User command handlers
 */
static void	command_share(server_t* self, char* path)
{
  if (!self->initialized) die("command_share: server has not been initialized");
  /* Alloc command
   */
  struct lst_excmd* new_cmd = malloc(sizeof(struct lst_excmd));
  if (!new_cmd)
    return; /* Forget cmd */
  char* dup_path = strdup(path);
  if (!dup_path)
    {
      free(new_cmd);
      return; /* Forget cmd */
    }
  new_cmd->command = SHARE;
  new_cmd->data.share_arg.path = dup_path;
  EXCMD_LOCK();
  LIST_INSERT_HEAD(&self->extern_commands, new_cmd, excmd_list); /* Insert in list */
  EXCMD_UNLOCK();
}

static void	command_register(server_t* self, char* addr, short port, unsigned id, char* path)
{
  if (!self->initialized) die("command_share: server has not been initialized");
  /* Alloc command
   */
  struct lst_excmd* new_cmd = malloc(sizeof(struct lst_excmd));
  if (!new_cmd)
    return; /* Forget cmd */
  char* dup_path = strdup(path);
  if (!dup_path)
    {
      free(new_cmd);
      return; /* Forget cmd */
    }
  char* dup_addr = strdup(addr);
  if (!dup_addr)
    {
      free(new_cmd);
      free(dup_path);
      return; /* Forget cmd */
    }
  new_cmd->command = REGISTER;
  new_cmd->data.register_arg.addr = dup_addr;
  new_cmd->data.register_arg.port = port;
  new_cmd->data.register_arg.id = id;
  new_cmd->data.register_arg.path = dup_path;
  EXCMD_LOCK();
  LIST_INSERT_HEAD(&self->extern_commands, new_cmd, excmd_list); /* Insert in list */  
  EXCMD_UNLOCK();
}

/* On command handlers
 */
static int		share(server_t* self, char* path)
{
  if (self->get_elem_by_path(self, path))
    return -1; /* Elem already shared */
  elem_t* local = alloc_elem();
  if (local->init_with_path(local, path)) return -1; /* die("share: init_with_path"); */
  insert_new_elem(self, local);
  return 0;
}

static int		synchronize(server_t* self, char* src_addr, short src_port, unsigned dist_id, char* path)
{
  /* Get host
   */
  host_t* src = NULL;
  struct lst_host* lst_host = NULL;
  LIST_FOREACH(lst_host, &self->hosts, host_list)
    {
      host_t* host = lst_host->host;
      if (host->addr && !strcmp(src_addr, host->addr) && host->port == src_port)
	{
	  src = host;
	  break;
	}
    }
  if (!src) /* Host not found, try to create it */
    {
      src = alloc_host();
      if (src->init_with_addr(src, src_addr, src_port)) return -1; /* die("synchronize: init_with_addr failed"); */
      insert_new_host(self, src);
    }
  elem_t* local = self->get_elem_by_path(self, path);
  if (!local) /* Elem not found */
    {
      local = alloc_elem();
      if (local->init_with_path(local, path)) return -1; /* die("synchronize: init_with_path failed"); */
      insert_new_elem(self, local);
    }
  dist_elem_t* target = alloc_dist_elem();
  target->init_with_id_src_and_elem(target, dist_id, src, local);
  insert_new_dist_elem(self, target);
  return 0;
}

static int		server_timeout(server_t* self, unsigned timo)
{
  return -1;
}
