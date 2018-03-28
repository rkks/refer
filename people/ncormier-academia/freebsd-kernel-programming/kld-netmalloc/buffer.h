/*
** buffer.h for  in /home/nico/lang/kernel/kld-buffertrigger
** 
** Made by nicolas
*/

#ifndef __BUFFER_H__
# define __BUFFER_H__

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
buffer_t*		alloc_buffer(void);

/* Buffer struct
 */
struct buffer
{
  /* Properties
   */
  char*		buf;
  unsigned	buf_size;
  /* Methods
   */
  int				(*init_with_size)(buffer_t* self, unsigned size);
  void				(*destroy)(buffer_t* self);
  int				(*read)(buffer_t* self, unsigned offset, char* buf, unsigned size);
  int				(*write)(buffer_t* self, unsigned offset, char* buf, unsigned size);
  unsigned			(*size)(buffer_t* self);
};

#endif /* __BUFFER_H__ */
