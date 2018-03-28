/*
** elem.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __ELEM_H__
# define __ELEM_H__

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
elem_t*		alloc_elem(void);
dist_elem_t*	alloc_dist_elem(void);

/* Elem struct
 */
struct elem
{
  /* Properties
   */
  unsigned			id; /* Elem id */
  buffer_t*			buffer; /* Data */
  /* Methods
   */
  int				(*init_with_size)(elem_t* self, unsigned size);
  void				(*destroy)(elem_t* self);
  void				(*send_contents)(elem_t* self, unsigned offset, host_t* to); /* get-command  handler */
  void				(*update_contents)(elem_t* self, char* contents, unsigned size, unsigned at_offset, host_t* from); /* contents-command handler */
};

/* Dist elem struct
 */
struct dist_elem
{
  /* Properties
   */
  unsigned	id; /* Distant elem id */
  host_t*	src; /* Distant src */
  elem_t*	local_elem; /* Mirror */
  enum {
    SYNCHRONIZED = 0,
    SYNCHRONIZING
  }		status;
  int		current_concat_offset;
  /* Methods
   */
  void		(*init_with_id_size_and_host)(dist_elem_t* self, unsigned id, unsigned size, host_t* src);
  void		(*destroy)(dist_elem_t* self);
  void		(*query_update)(dist_elem_t* self); /* Send get-command */
  void		(*update_contents)(dist_elem_t* self, char* contents, unsigned size, host_t* from); /* contents-command handler */
};


#endif /* __ELEM_H__ */
