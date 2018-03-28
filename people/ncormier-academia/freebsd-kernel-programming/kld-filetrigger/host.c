/*
** host.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
#else
# include <sys/libkern.h>
# include "kernel-dependant.h"
#endif /* _KERNEL */
#include "host.h"
#include "elem.h"
#include "server.h"
#include "socket.h"
#include "die.h"

/* Forward declarations
 */
static int	host_init_with_socket(host_t* self, socket_t* sock);
static int	host_init(host_t* self, const char* addr, short port);
static void	host_destroy(host_t* self);
static void	host_recv(host_t* self);
static void	host_send(host_t* self);
static void	host_add_chunk_contents(host_t* self, unsigned id, char* buftocpy, unsigned max_size);
static void	host_query_update(host_t* self, unsigned id);
static void	host_query_next_chunk_for_id_at_offset(host_t* self, unsigned id, unsigned offset);

/* Extern functions
 */
host_t*	alloc_host(void)
{
  host_t* host = malloc(sizeof(host_t));
  if (!host)
    return NULL;
  bzero(host, sizeof(host_t));
  /* Init methods
   */
  host->init_with_socket = host_init_with_socket;
  host->init_with_addr = host_init;
  host->destroy = host_destroy;
  host->recv = host_recv;
  host->send = host_send;
  host->add_chunk_contents = host_add_chunk_contents;
  host->query_update = host_query_update;
  host->query_next_chunk_for_id_at_offset = host_query_next_chunk_for_id_at_offset;
  return host;
}

/* Local functions
 */
static struct output_chunk*	alloc_chunk_with_data(char* data, unsigned size)
{
  struct output_chunk* chunk = malloc(sizeof(struct output_chunk));
  if (!chunk)
    return NULL;
  chunk->rsize = size;
  if (size > CHUNK_SIZE)
    size = CHUNK_SIZE;
  (void) memcpy(chunk->contents, data, size);
  return chunk;
}

static struct output_chunk*	alloc_chunk_with_header_and_data(char* header, unsigned header_size, char* data, unsigned data_size)
{
  struct output_chunk* chunk = malloc(sizeof(struct output_chunk));
  if (!chunk)
    return NULL;
  unsigned size = header_size + data_size;
  chunk->rsize = size;
  if (size > CHUNK_SIZE)
    size = CHUNK_SIZE;
  (void) memcpy(chunk->contents, header, header_size);
  (void) memcpy(chunk->contents + header_size, data, data_size);
  return chunk;
}

static void	add_chunk_to_output_fifo(host_t* self, struct output_chunk* chunk)
{
  LIST_INSERT_HEAD(&self->output_fifo, chunk, chunks_list);
}

static int	host_init_with_socket(host_t* self, socket_t* sock)
{
  /* Init members
   */
  self->socket = sock;
  LIST_INIT(&self->output_fifo);
  return 0;
}

static int	host_init(host_t* self, const char* addr, short port)
{
  /* Try to connect
   */
  char* dup_addr = strdup(addr);
  if (!dup_addr)
    return -1;
  socket_t* sock = alloc_socket();
  if (!sock)
    {
      free(dup_addr);
      return -1;
    }
  if (sock->init_and_connect_to(sock, addr, port) == -1)
    {
      free(dup_addr);
      sock->destroy(sock);
      return -1;
    }
  /* Init members
   */
  self->addr = dup_addr;
  self->port = port;
  return host_init_with_socket(self, sock);
}

static void	host_destroy(host_t* self)
{
  /* Cleanup
   */
  struct output_chunk* chunk = NULL;
  LIST_FOREACH(chunk, &self->output_fifo, chunks_list)
    free(chunk);
  if (self->socket)
    self->socket->destroy(self->socket);
  if (self->addr)
    free(self->addr);
  free(self);
}

static void	host_recv(host_t* self)
{
  int buf[3];
  int read_ret = self->socket->read(self->socket, (char*)buf, sizeof(buf));
  if (read_ret == -1 || read_ret < sizeof(buf)) die("host_recv: can't read socket");
  switch (buf[0])
    {
    case GET:
      {
	unsigned id = (unsigned) buf[1];
	unsigned offset = (unsigned) buf[2];
	elem_t* elem = server_get_elem_by_id(id);
	if (elem)
	  elem->send_contents(elem, offset, self);
	else
	  {
	    /* TODO: send no such file or directory
	     */
	    die("host_recv: can't find elem");
	  }
      }
      break;
    case CONTENTS:
      {
	unsigned id = (unsigned) buf[1];
	unsigned size = (unsigned) buf[2];
	dist_elem_t* dist_elem = server_get_dist_elem_by_id(id);
	if (!dist_elem) 
	  {
	    /* TODO: just forbid command ?
	     */
	    die("host_recv: can't find dist_elem");
	  }
	char data_buf[CHUNK_SIZE];
	int read_ret = self->socket->read(self->socket, data_buf, size);
	if (read_ret == -1 || read_ret < size) die("host_recv: can't read socket");
	dist_elem->update_contents(dist_elem, data_buf, read_ret, self); /* Concat data in file and query for next chunk */
      }
      break;
    default:
      die("host_recv: unknown commad");
    }
}

static void	host_send(host_t* self)
{
  struct output_chunk* chunk = LIST_FIRST(&self->output_fifo);
  if (chunk)
    {
      int write_ret = self->socket->write(self->socket, chunk->contents, chunk->rsize);
      if (write_ret == -1)
	return;
      if (write_ret < chunk->rsize) die("TODO: partial send");
      LIST_REMOVE(chunk, chunks_list); /* Remove from the list */
    }
}

static void	host_add_chunk_contents(host_t* self, unsigned id, char* buftocpy, unsigned max_size)
{
  int contents_cmd[3];
  unsigned data_size = CHUNK_SIZE - sizeof(contents_cmd);
  if (max_size < data_size)
    data_size = max_size;
  contents_cmd[0] = CONTENTS;
  contents_cmd[1] = id;
  contents_cmd[2] = data_size;
  struct output_chunk* chunk = alloc_chunk_with_header_and_data((char*)contents_cmd, sizeof(contents_cmd), buftocpy, data_size);
  if (!chunk) die("host_add_chunk_contents: chunk allocation failed");
  add_chunk_to_output_fifo(self, chunk);
}

static void	host_query_update(host_t* self, unsigned id)
{
  host_query_next_chunk_for_id_at_offset(self, id, 0);
}


static void	host_query_next_chunk_for_id_at_offset(host_t* self, unsigned id, unsigned offset)
{
  int get_next_chunk_cmd[3];
  get_next_chunk_cmd[0] = GET;
  get_next_chunk_cmd[1] = id;
  get_next_chunk_cmd[2] = offset;
  struct output_chunk* chunk = alloc_chunk_with_data((char*)get_next_chunk_cmd, sizeof(get_next_chunk_cmd));
  if (!chunk) die("host_add_chunk_contents: chunk allocation failed");
  add_chunk_to_output_fifo(self, chunk);
}
