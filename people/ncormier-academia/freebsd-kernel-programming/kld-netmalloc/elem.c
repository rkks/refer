/*
** elem.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <strings.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
#else
# include <sys/libkern.h>
# include <sys/fcntl.h>
# include "kernel-dependant.h"
#endif /* _KERNEL */
#include "elem.h"
#include "host.h"
#include "die.h"
#include "buffer.h"
#include "server.h"

/* Forward declarations
 */
static int	elem_init(elem_t* self, unsigned size);
static void	elem_destroy(elem_t* self);
static void	elem_send_contents(elem_t* self, unsigned offset, host_t* to);
static void	elem_update_contents(elem_t* self, char* contents, unsigned size, unsigned at_offset, host_t* from);
static void	dist_elem_init_with_id_size_and_host(dist_elem_t* self, unsigned id, unsigned size, host_t* src);
static void	dist_elem_destroy(dist_elem_t* self);
static void	dist_elem_query_update(dist_elem_t* self);
static void	dist_elem_update_contents(dist_elem_t* self, char* contents, unsigned size, host_t* from);

/* Extern functions
 */
elem_t*	alloc_elem(void)
{
  elem_t* elem = malloc(sizeof(elem_t));
  if (!elem)
    return NULL;
  bzero(elem, sizeof(elem_t));
  /* Init methods
   */
  elem->init_with_size = elem_init;
  elem->destroy = elem_destroy;
  elem->send_contents = elem_send_contents;
  elem->update_contents = elem_update_contents;
  return elem;
}

dist_elem_t*	alloc_dist_elem(void)
{
  dist_elem_t* elem = malloc(sizeof(dist_elem_t));
  if (!elem)
    return NULL;
  bzero(elem, sizeof(dist_elem_t));
  /* Init methods
   */
  elem->init_with_id_size_and_host = dist_elem_init_with_id_size_and_host;
  elem->destroy = dist_elem_destroy;
  elem->query_update = dist_elem_query_update;
  elem->update_contents = dist_elem_update_contents;
  return elem;
}

/* Local functions
 */
static int	elem_init(elem_t* self, unsigned size)
{
  static int	local_id_generator = 0;
  /* Create buffer
   */
  buffer_t* buffer = alloc_buffer();
  if (!buffer)
    return -1;
  if (buffer->init_with_size(buffer, size) == -1)
    {
      buffer->destroy(buffer);
      return -1;
    }
  /* Init members
   */
  self->id = local_id_generator++;
  self->buffer = buffer;
  return 0;
}

static void	elem_destroy(elem_t* self)
{
  /* Cleanup
   */
  if (self->buffer)
    self->buffer->destroy(self->buffer);
  free(self);
}

#define READBUF_SIZE	256
static void	elem_send_contents(elem_t* self, unsigned offset, host_t* to)
{
  if (!self->buffer) die("elem_send_contents: buffer has not been initialized");
  /* Check offset validity
   */
  unsigned size = self->buffer->size(self->buffer);
  if (size == (unsigned)-1) die("elem_send_contents: can't get buffer size");
  if (offset > size)
    {
      to->add_chunk_contents(to, self->id, NULL, 0); /* Send eof to client */
      return;
    }
  unsigned bytes_left = size - offset;
  /* Read buffer at offset
   */
  char* readbuf = malloc(READBUF_SIZE);
  int read_ret = self->buffer->read(self->buffer, offset, readbuf, READBUF_SIZE);
  if (read_ret == -1) die("elem_send_contents: can't read buffer");
  if (read_ret > bytes_left)
    read_ret = bytes_left;
  to->add_chunk_contents(to, self->id, readbuf, read_ret); /* Send query chunk */
  free(readbuf);
}

static void	elem_update_contents(elem_t* self, char* contents, unsigned size, unsigned at_offset, host_t* from)
{
  /* Write data in buffer
   */
  int write_ret = self->buffer->write(self->buffer, at_offset, contents, size);
  if (write_ret == -1 || write_ret < size) die("elem_update_contents: can't write buffer");
}

/* Dist elem implementation
 */

static void	dist_elem_init_with_id_size_and_host(dist_elem_t* self, unsigned id, unsigned size, host_t* src)
{
  elem_t* elem = alloc_elem();
  if (!elem) die("dist_elem_init_with_id_size_and_host: elem allocation failed");
  if (elem->init_with_size(elem, size) == -1) die("dist_elem_init_with_id_size_and_host: elem initialization failed");
  /* Init members
   */
  if (id == (unsigned)-1) /* Same id has local */
    self->id = elem->id;
  else
    self->id = elem->id = id;
  printf("dist=%d local=%d\n", self->id, elem->id);
  self->src = src;
  self->local_elem = elem;
  self->status = SYNCHRONIZED;
  self->current_concat_offset = 0;
}

static void	dist_elem_destroy(dist_elem_t* self)
{
  /* Cleanup
   */
  free(self);
}

static void	dist_elem_query_update(dist_elem_t* self)
{
  /* Query update
   */
  self->src->query_update(self->src, self->id);
}

static void	dist_elem_update_contents(dist_elem_t* self, char* contents, unsigned size, host_t* from)
{
  /* End of update ?
   */
  if (!size)
    {
      self->current_concat_offset = 0;
      self->status = SYNCHRONIZED;
      get_server()->on_update_finished(get_server(), self->id); /* Notify server */
      return;
    }
  /* New buffer version ?
   */
  if (self->status == SYNCHRONIZED)
    {
      self->current_concat_offset = 0;
      self->status = SYNCHRONIZING;
    }
  /* Update on-disk elem contents
   */
  self->local_elem->update_contents(self->local_elem, contents, size, self->current_concat_offset, from);
  /* Update offset & query next chunk
   */
  self->current_concat_offset += size;
  from->query_next_chunk_for_id_at_offset(from, self->id, self->current_concat_offset);
}
