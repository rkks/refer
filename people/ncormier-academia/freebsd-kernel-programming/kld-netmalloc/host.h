/*
** host.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __HOST_H__
# define __HOST_H__

# include <sys/queue.h>

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
host_t*	alloc_host(void);

/* Private typedef
 */
struct lst_syncelem
{
  elem_t*			syncelem;
  int				bytes_left;
};

struct output_chunk
{
# define CHUNK_SIZE		256
  unsigned			rsize;
  char				contents[CHUNK_SIZE];
  LIST_ENTRY(output_chunk)	chunks_list;
};

enum command_types {
  GET = 0,
  CONTENTS,
  CREATE_REQUEST,
  CREATE_ANSWER
};

/* Host struct
 */
struct host
{
  socket_t*			socket;
  char*				addr;
  short				port;
  LIST_HEAD(, output_chunk)	output_fifo;
  /* Methods
   */
  int				(*init_with_socket)(host_t* self, socket_t* sock);
  int				(*init_with_addr)(host_t* self, const char* addr, short port);
  void				(*destroy)(host_t* self);
  void				(*recv)(host_t* self);
  void				(*send)(host_t* self);
  void				(*add_chunk_contents)(host_t* self, unsigned id, char* buftocpy, unsigned max_size);
  void				(*query_allocation)(host_t* self, unsigned size);
  void				(*query_update)(host_t* self, unsigned id);
  void				(*query_next_chunk_for_id_at_offset)(host_t* self, unsigned id, unsigned offset);
};


#endif /* __HOST_H__ */
