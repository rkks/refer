/*
** low_layer.c for  in /home/nico/lang/c/ftrace/lib/freebsd_ia32/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:56:27 2006 nicolas
** Last update Tue Apr 25 20:50:52 2006 nicolas cormier
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/core.h>

#include "coffobj.h"

#include "includes/error.h"

#include "api/includes/prototypes/core/low_layer.h"

core_obj_t*		core_new(char* path)
{
  struct stat		sb;
  int			fd;
  void*			data;
  core_obj_t*		ret;
  osfcoreobj_t*		luse;
  int			i = 0;
  struct core_filehdr   *core_header;

  /*
  ** Map the core.
  */

  if ((fd = open(path, O_RDONLY)) == -1)
    {
      return (NULL);
    }
  if (fstat(fd, &sb) == -1)
    {
      close(fd);
      return (NULL);
    }
  data = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  close(fd);
  if (data == MAP_FAILED)
    return (NULL);

  core_header = (struct core_filehdr *) data;
  /*
  ** Check if the core is really an OSF1-Core.
  */

  if (strncmp(core_header->magic, MAGIC_COFF_CORE, 4) == 0)
    {
      errno = EFTRACE;
      ftrace_errstr = "File is not an OSF1 core file.";
      munmap(data, sb.st_size);
      return (NULL);
    }

  /*
  ** Fill the return
  */

  ret = malloc(sizeof(core_obj_t));
  if (ret == NULL)
    {
      munmap(data, sb.st_size);
      return (NULL);
    }
  ret->path = strdup(path);
  ret->map = data;
  ret->fd = -1;
  ret->size = sb.st_size;
  ret->luse = NULL;

  /*
  ** Fill luse.
  */

  luse = malloc(sizeof(osfcoreobj_t));
  if (luse == NULL)
    return (ret);
  luse->core_header = (struct core_filehdr *) data;
  luse->core_section_headers = malloc(sizeof(struct core_scnhdr *) * luse->core_header->nscns);
  if (luse->core_section_headers == NULL)
    return (ret);

  /*
  ** Section headers.
  */

  for (i = 0; i < luse->core_header->nscns; i++)
    luse->core_section_headers[i] = (void*)((data + sizeof(struct core_filehdr)) + (i * sizeof(struct core_scnhdr)));
  luse->core_section_headers[i] = NULL;

  ret->luse = luse;
  return (ret);
}

int		core_del(core_obj_t* obj)
{
  osfcoreobj_t*	luse;

  if (obj->path)
    free(obj->path);
  if (obj->map)
    {
      if (munmap(obj->map, obj->size) == -1)
        return (-1);
    }
  if (obj->fd == -1)
    {
      if (close(obj->fd) == -1)
        return (-1);
    }
  if (obj->luse)
    {
      luse = obj->luse;
      if (luse->core_section_headers)
        free(luse->core_section_headers);
      free(luse);
    }
  free(obj);
  return (0);
}

int		core_read(core_obj_t* obj, addr_t addr, unsigned int len, char* laddr)
{
  unsigned long	i, ul, r = 0;
  char*		data;
  osfcoreobj_t*	luse;

  luse = (osfcoreobj_t *) obj->luse;
  while (len)
    {
      for (i = 0; i < luse->core_header->nscns; i++)
        {
          if (luse->core_section_headers[i]->scntype == SCNRGN &&
	      (ulong) luse->core_section_headers[i]->vaddr <= addr &&
              (ulong) (luse->core_section_headers[i]->vaddr +
		       luse->core_section_headers[i]->size) > addr)
            break;
        }
      if (i < luse->core_header->nscns)
        {
          ul = MIN((ulong) luse->core_section_headers[i]->vaddr +
		   luse->core_section_headers[i]->size - addr, len);
	  /*
	  ** FIXME : gros doute pour trouver data -- a revoir
	  */
          data = obj->map + (ulong) luse->core_section_headers[i]->vaddr + (ulong) addr;
          memcpy((char *) laddr + r, data, ul);
          len -= ul;
          r += ul;
        }
      else
	return (r);
    }
  memcpy(laddr, obj->map + (unsigned int) addr, len);
  return (0);
}
