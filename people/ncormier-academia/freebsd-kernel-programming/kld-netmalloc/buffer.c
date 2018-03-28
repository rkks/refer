/*
** buffer.c for  in /home/nico/lang/kernel/kld-buffertrigger
** 
** Made by nicolas
*/

/* Headers
 */
#ifndef _KERNEL
# include <stdlib.h>
# include <string.h>
# include <strings.h>
#else
# include <sys/libkern.h>
# include "kernel-dependant.h"
#endif /* _KERNEL */
#include "buffer.h"
#include "die.h"

/* Forward declarations
 */
static int	buffer_init_with_size(buffer_t* self, unsigned size);
static void	buffer_destroy(buffer_t* self);
static int	buffer_read(buffer_t* self, unsigned offset, char* buf, unsigned size);
static int	buffer_write(buffer_t* self, unsigned offset, char* buf, unsigned size);
static unsigned	buffer_size(buffer_t* self);

/* Extern functions
 */
buffer_t*	alloc_buffer(void)
{
  buffer_t* buffer = malloc(sizeof(buffer_t));
  if (!buffer)
    return NULL;
  bzero(buffer, sizeof(buffer_t));
  /* Init methods
   */
  buffer->init_with_size = buffer_init_with_size;
  buffer->destroy = buffer_destroy;
  buffer->read = buffer_read;
  buffer->write = buffer_write;
  buffer->size = buffer_size;
  return buffer;
}

/* Local functions
 */
static int	buffer_init_with_size(buffer_t* self, unsigned size)
{
  /* Allocate buffer
   */
  self->buf = malloc(size);
  if (!self->buf)
    return -1;
  self->buf_size = size;
  return 0;
}

static void	buffer_destroy(buffer_t* self)
{
  /* Cleanup
   */
  if (self->buf)
    free(self->buf);
  free(self);
}

static int	buffer_read(buffer_t* self, unsigned offset, char* buf, unsigned size)
{
  if (self->buf_size - offset < size)
    size = self->buf_size - offset;
  (void) memcpy(buf, self->buf + offset, size);
  return size;
}

static int	buffer_write(buffer_t* self, unsigned offset, char* buf, unsigned size)
{
  if (self->buf_size - offset < size) die("buffer_write: too much data");
  (void) memcpy(self->buf + offset, buf, size);
  return size;
}

static unsigned	buffer_size(buffer_t* self)
{
  return self->buf_size;
}
