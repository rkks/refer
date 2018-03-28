/*
** high_layer.c for  in /home/nico/lang/c/ftrace/src/api/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 12:58:15 2006 nicolas
** Last update Tue Apr 25 23:30:39 2006 nicolas
*/

#include <stdlib.h>
#include <string.h>
#ifdef __NetBSD__
#include <sys/types.h>
#include <sys/ptrace.h>
#endif /* __NetBSD__ */
#ifdef __FreeBSD__
#include <sys/procfs.h>
#endif /* __FreeBSD__ */
#include <sys/param.h>
#ifdef __NetBSD__
#define Elf_Note	Elf_Nhdr
#define roundup2	roundup
#endif /* __NetBSD__ */
#include <elf.h>
#include <sys/link_elf.h>
#undef __CC_SUPPORTS_WARNING
#include <sys/syslimits.h>  /* PATH_MAX */
#include <errno.h>

#include "elfobj.h"
#include "includes/error.h"

#include "api/includes/prototypes/core/high_layer.h"
#include "api/includes/prototypes/core/low_layer.h"
#include "api/includes/prototypes/prim_types.h"

#include "api/includes/types/prim_types.h"
#include "api/includes/types/backtrace.h"


/*
** Get the contain of a note.
*/
static int	cpy_frm_elfnote(core_obj_t* obj, char* note_name, unsigned int note_type, void* buf, int len)
{
  int		i;
  Elf_Note*	note;
  char*		start, * end, * cur_name, * data;
  elfobj_t*	luse;


  luse = (elfobj_t*) obj->luse;

  for (i = 0; i < luse->header->e_phnum; i++)
    {
      if (luse->program_headers[i]->p_type == PT_NOTE)
	{
	  start = obj->map + luse->program_headers[i]->p_offset;
	  end = start + luse->program_headers[i]->p_filesz;
	  for (; start < end; )
	    {
	      note = (Elf_Note*) start;
	      start += sizeof(Elf_Note);
	      cur_name = start;
	      start += roundup2(note->n_namesz, 4);
	      data = start;
	      start += roundup2(note->n_descsz, 4);
	      if (!strcmp(note_name, cur_name) && note->n_type == note_type)
		{
		  memcpy(buf, data, len);
		  return (len);
		  /* *buf = data; */
		  /* *len = note->n_descsz; */
		}
	    }
	}
    }
  return (-1);
}

regs_t*		core_get_registers(core_obj_t* obj)
{
  struct reg	cur_reg;
  regs_t*	regs;

#ifdef __FreeBSD__
  prstatus_t	prstatus;

  if (cpy_frm_elfnote(obj, "FreeBSD", NT_PRSTATUS, &prstatus, sizeof(prstatus_t)) == -1)
    return (NULL);
  cur_reg = prstatus.pr_reg;
#endif /* __FreeBSD__ */


#ifdef __NetBSD__
  if (cpy_frm_elfnote(obj, "NetBSD-CORE@1", PT_GETREGS, &cur_reg, sizeof(struct reg)) == -1)
    return (NULL);
#endif /* __NetBSD__ */


  regs = malloc(sizeof(regs_t));
  if (regs == NULL)
    return (NULL);
  memcpy(regs, &cur_reg, sizeof(struct reg));

  return (regs);
}

#ifdef __NetBSD__
#include <sys/exec_elf.h>
#endif /* __NetBSD__ */
char*		core_get_bin_path(core_obj_t* obj)
{
#ifdef __NetBSD__
  struct netbsd_elfcore_procinfo	procinfo;

  if (cpy_frm_elfnote(obj, "NetBSD-CORE", ELF_NOTE_NETBSD_CORE_PROCINFO, &procinfo,
  sizeof(struct netbsd_elfcore_procinfo)) == -1)
    errexit("backtrace: Can't read bin image name from core !");
  return (strdup(procinfo.cpi_name));
#endif /* __NetBSD__ */

#ifdef __FreeBSD__
  prpsinfo_t	prpsinfo;

  if (cpy_frm_elfnote(obj, "FreeBSD", NT_PRPSINFO, &prpsinfo, sizeof(prpsinfo_t)) == -1)
    return (NULL);
  return (strdup(prpsinfo.pr_fname));
#endif /* __FreeBSD__ */

  return (NULL);
}

list_t*	core_get_depends_list(core_obj_t* obj, bin_obj_t* binobj)
{
  struct r_debug		rdebug;
  int			i;
  Elf_Addr		dyn_ptr;
  Elf_Dyn*		dyn;
  Elf_Dyn*		debug = 0;
  elfobj_t*		luse;
  Elf_Addr		addr;
  char			buf[PATH_MAX + 1];
  struct link_map	map;
  list_t*		ret = NULL;
  depend_t*		cur = NULL;

  if (!obj || !binobj)
    return (NULL);

  luse = (elfobj_t*) binobj->luse;

  /*
  ** Get debug address in the core.
  */
  for (i = 0; luse && i < luse->header->e_phnum; i++)
    {

      if (luse->program_headers[i]->p_type == PT_DYNAMIC)
	{
	  for (dyn_ptr = 0; dyn_ptr < luse->program_headers[i]->p_filesz;
	       dyn_ptr += sizeof(Elf_Dyn))
	    {
	      dyn = binobj->map + luse->program_headers[i]->p_offset + dyn_ptr;
	      if (dyn->d_tag == DT_DEBUG)
		{
		  if (!(debug = alloca(sizeof(Elf_Dyn))))
		    return (NULL);
		  if (core_read(obj, luse->program_headers[i]->p_vaddr + dyn_ptr,
				sizeof(Elf_Dyn), (char*) debug) == -1)
		    debug = 0;
		  break;
		}
	    }
	  break;
	}
    }
  if (debug == 0)
    return (NULL);
  /*
  ** Copy debug strut from the traced process.
  */
  if (core_read(obj, debug->d_un.d_ptr, sizeof(struct r_debug),
		(char*) &rdebug) == -1)
    return (NULL);
  addr = (Elf_Addr) rdebug.r_map;
  /*
  ** Browse linkmap & add to return list.
  */
  for (; addr; addr = (Elf_Addr) map.l_next)
    {
      if (core_read(obj, addr, sizeof(struct link_map), (char*) &map) == -1)
	break;
      if ((Elf_Addr) map.l_addr <= luse->header->e_entry)
	continue;
      if (map.l_name == 0)
	continue;
      if (core_read(obj, (long) map.l_name, PATH_MAX, (char*) buf) == -1)
	continue;
      cur = malloc(sizeof(depend_t));
      if (cur == NULL)
	goto core_get_depends_list_failed;
      cur->path = strdup(buf);
      cur->base_addr = (addr_t) map.l_addr;
      ret = list_add(ret, cur);
      if (ret == NULL)
	goto core_get_depends_list_failed;
    }
  return (ret);

 core_get_depends_list_failed:
  for (; ret; )
    {
      cur = (depend_t*) ret->value;
      ret = list_del(ret, cur);
      free(cur);
    }
  return (NULL);
}


/*
** Backtrace recursive function.
*/
static int	recur_stack_call_trace(core_obj_t* obj, regs_t* cur_reg, int count, list_t** listret)
{
  int		ret;
  caddr_t	cur = (caddr_t) cur_reg->r_ebp;
  caddr_t	eip, ebp;
  backt_t*	backt;

  eip = (caddr_t) cur_reg->r_eip;
  ebp = (caddr_t) cur_reg->r_ebp;

  if (core_read(obj, (long) cur, sizeof(int), (char*) &ret) == -1)
    return (-1);

  cur_reg->r_ebp = ret;
  if ((caddr_t) ret <= ebp)
    return (0);

  if (core_read(obj, (long)(cur + sizeof(int)), sizeof(int), (char*) &ret) == -1)
    return (-1);

  if (ret == 0)
    return (0);

  backt = malloc(sizeof(backt_t));
  if (backt == NULL)
    return (-1);

  backt->addr = (addr_t) ret;
  backt->count = count;

  *listret = list_add(*listret, backt);
  if (*listret == NULL)
    return (-1);

  if (recur_stack_call_trace(obj, cur_reg, count + 1, listret) == -1)
    return (-1);

  return (0);
}

backtrace_t*	core_backtrace(core_obj_t* obj)
{
  list_t*	ret = NULL;
  regs_t*	cur_reg = NULL;
  backt_t*	backt;

  cur_reg = core_get_registers(obj);
  if (cur_reg == NULL)
    goto core_backtrace_failed;

  backt = malloc(sizeof(backt_t));
  if (backt == NULL)
    goto core_backtrace_failed;

  backt->addr = (addr_t) cur_reg->r_eip;
  backt->count = 0;

  if (recur_stack_call_trace(obj, cur_reg, 1, &ret) == -1)
    goto core_backtrace_failed;

  return (ret);

  /*
  ** If failed, free list And return NULL.
  */
 core_backtrace_failed:
  if (ret)
    free(ret);
  if (cur_reg)
    free(cur_reg);
  for (; ret; )
    {
      backt = (backt_t*) ret->value;
      ret = list_del(ret, backt);
      free(backt);
    }
  return (NULL);
}
