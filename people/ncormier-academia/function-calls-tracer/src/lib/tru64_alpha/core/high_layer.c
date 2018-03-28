/*
** high_layer.c for  in /home/nico/lang/c/ftrace/src/api/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 12:58:15 2006 nicolas
** Last update Tue Apr 25 21:19:01 2006 nicolas cormier
*/

#include <stdlib.h>
#include <string.h>

#include "coffobj.h"

#include "api/includes/prototypes/core/high_layer.h"

/*
** A TESTER
*/

regs_t*	core_get_registers(core_obj_t* obj)
{
  int			i;
  regs_t		*regs;
  osfcoreobj_t		*luse;

  luse = (osfcoreobj_t *) obj->luse;
  if (luse == NULL)
    return (NULL);
  regs = malloc(sizeof(regs_t));
  if (regs == NULL)
    return (NULL);

  for (i = 0; i < luse->core_header->nscns; i++)
    {
      if (luse->core_section_headers[i]->scntype == SCNREGS)
	{
	  memcpy(regs, obj->map + (uint) *((uint*)luse->core_section_headers[i]->vaddr), luse->core_section_headers[i]->size);
	  return (regs);
	}
    }
  return (NULL);
}

/*
** TODO: Seulement le nom du binaire et non le path entier
*/
char*		core_get_bin_path(core_obj_t* obj)
{
  char		*name;
  osfcoreobj_t	*luse;

  luse = (osfcoreobj_t *) obj->luse;
  if (luse == NULL)
    return (NULL);
  name = strdup(luse->core_header->name);
  return (name);
}

depends_t*	core_get_depends_list(core_obj_t* obj, bin_obj_t* bin)
{
  /*
  ** Probablement meme methode que Bin.
  ** Point fixe et faire un core_read dessus depuis la low_layer,
  ** mais il faut un truc qui marche sur les binaires avant pour tester la methode
  ** Donc devrait etre facile une fois l'autre termine.
  */
  obj = NULL;
  bin = NULL;
  return (NULL);
}

backtrace_t*      core_backtrace(core_obj_t* obj)
{
  obj = NULL;
  return (NULL);
}
