/*
** file.h for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef __FILE_H__
# define __FILE_H__

/* Forward declaration
 */
# include "forward_typedefs.h"

/* Functions
 */
file_t*		alloc_file(void);

/* Private typedef
 */
# ifndef _KERNEL
#  include <dirent.h>
struct pv
{
  int	fd;
  DIR*	dir;
};
# else
struct pv
{
  struct vnode*	vp;
};
# endif /* _KERNEL */

/* File struct
 */
struct file
{
  /* Properties
   */
  struct pv*	pvdata; /* Abstract data */
  char*		path;
  /* Methods
   */
  int				(*init_with_path)(file_t* self, const char* path, int flags, int cmode);
  void				(*destroy)(file_t* self);
  int				(*read)(file_t* self, unsigned offset, char* buf, unsigned size);
  int				(*write)(file_t* self, unsigned offset, char* buf, unsigned size, void* pv);
  unsigned			(*size)(file_t* self);
  int				(*truncate)(file_t* self, unsigned new_size);
};

#endif /* __FILE_H__ */
