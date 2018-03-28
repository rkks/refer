/*
** elf.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Sat Jan  7 12:41:54 2006 nicolas
** Last update Wed Jan 25 09:56:42 2006 nicolas cormier
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "elffprot.h"
#include "backtrace.h"

extern int	son;
extern int	verbose;
static elfobj*	elfobjects = NULL;
static elfobj*	elfcore = NULL;


void	free_elfobjects(void)
{
  elfobj*	cur, * next;

  if (elfobjects)
    {
      for (cur = elfobjects; cur; cur = next)
	{
	  if (munmap(cur->data, cur->size) == -1)
	    fprintf(stderr, "backtrace: munmap, %s", strerror(errno));
	  next = cur->next;
	  free(cur);
	}
      elfobjects = NULL;
    }
  if (elfcore)
    {
      free(elfcore);
      elfcore = NULL;
    }
}

elfobj*	get_elfobj_list(void)
{
  return (elfobjects);
}

/*
**
** Create, fill and insert in the list an elf object.
**
*/
void	fill_elfobj(char* path, void* data, unsigned int size)
{
  int		i = 0;
  char*		ptr;
  elfobj*	obj;

  obj = malloc(sizeof(elfobj));
  if (!obj)
    errexit("backtrace: malloc, %s", strerror(errno));
  memcpy(obj->name, path, strlen(path) + 1);
  obj->data = data;
  obj->size = size;
  obj->header = (Elf_Ehdr*) data;
  if (!IS_ELF(*obj->header))
    errexit("backtrace: file is not an ELF image");
  /*
  ** Program headers.
  */
  obj->program_headers = malloc(sizeof(Elf_Phdr *) * (obj->header->e_phnum + 1));
  for (ptr = (char*) obj->header +obj->header->e_phoff, i = 0; i < obj->header->e_phnum; i++)
    {
      obj->program_headers[i] = (Elf_Phdr *) ptr;
      ptr += obj->header->e_phentsize;
    }
  obj->program_headers[i] = 0;
  /*
  ** Section headers.
  */
  obj->section_headers = malloc(sizeof(Elf_Shdr *) * (obj->header->e_shnum + 1));
  for (ptr = (char*) obj->header + obj->header->e_shoff, i = 0; i < obj->header->e_shnum; i++)
    {
      obj->section_headers[i] = (Elf_Shdr *) ptr;
      ptr += obj->header->e_shentsize;
    }
  obj->section_headers[i] = 0;
  /*
  ** Section strng.
  */
  if (obj->header->e_shstrndx != SHN_UNDEF)
    obj->section_str = data + obj->section_headers[obj->header->e_shstrndx]->sh_offset;
  else
    obj->section_str = 0;
  /*
  ** Add obj to the list.
  */
  if (!elfobjects)
    {
      elfobjects = obj;
      obj->next = NULL;
    }
  else
    {
      obj->next = elfobjects;
      elfobjects = obj;
    }
}

/*
**
** Open and map elf object.
**
*/
void	openelf(char* path)
{
  struct stat	sb;
  int		fd;
  void*		data;

  if ((fd = open(path, O_RDONLY)) == -1)
    {
      fprintf(stderr, "backtrace: can't open %s:, %s", path, strerror(errno));
      return;
    }
  if (fstat(fd, &sb) == -1)
    {
      close(fd);
      fprintf(stderr, "backtrace: can't stat %s: %s", path, strerror(errno));
      return;
    }
  data = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
  close(fd);
  if (data == MAP_FAILED)
    errexit("backtrace: mmap(PROT_READ, MAP_SHARED), %s", strerror(errno));
  if (verbose)
    printf("Open: %s on %p\n", path, data);
  else
    printf("Open: %s\n", path);

  fill_elfobj(path, data, sb.st_size);
}

#ifdef __NetBSD__
#include <link_elf.h>
#endif /* __NetBSD__ */
#ifdef __FreeBSD__
#include <sys/link_elf.h>
#endif /* __FreeBSD__ */
/*
**
** There is a DT_DEBUG entry in the dynamic section which
** contain the address of a debug struct in running process.
** struct r_debug have the link_map of the process.
**
*/
Elf_Addr	get_link_map_list(elfobj* bin)
{
  struct r_debug		rdebug;
  int				i;
  Elf_Addr			dyn_ptr;
  Elf_Dyn*			dyn;
  Elf_Dyn*			debug = 0;

  /*
  ** Get debug address in the traced process.
  */
  for (i = 0; elfobjects && i < elfobjects->header->e_phnum; i++)
    {
      if (elfobjects->program_headers[i]->p_type == PT_DYNAMIC)
	{
	  for (dyn_ptr = 0; dyn_ptr < elfobjects->program_headers[i]->p_filesz; dyn_ptr += sizeof(Elf_Dyn))
	    {
	      dyn = elfobjects->data + elfobjects->program_headers[i]->p_offset + dyn_ptr;
	      if (dyn->d_tag == DT_DEBUG)
		{
		  if (!(debug = alloca(sizeof(Elf_Dyn))))
		    errexit("backtrace: alloca(sizeof(Elf_Dyn)), %s", strerror(errno));
		  if (readmem_frm_exec(elfobjects->program_headers[i]->p_vaddr + dyn_ptr, sizeof(Elf_Dyn), (char*) debug) == -1)
		    debug = 0;
		  break;
		}
	    }
	  break;
	}
    }
  if (debug == 0)
    return (0);
  if (verbose)
    printf("Debug address found at %p\n", (char*) debug);
  /*
  ** Copy debug strut from the traced process.
  */
  if (readmem_frm_exec(debug->d_un.d_ptr, sizeof(struct r_debug), (char*) &rdebug) == -1)
    return (0);
  if (verbose)
    printf("Link map found at %p\n", (char*) rdebug.r_map);
  return ((Elf_Addr) rdebug.r_map);
}

void	load_elfcore(char* core)
{
  if (core)
    {
      openelf(core);
      elfcore = elfobjects;
      elfobjects = elfobjects->next;
    }
}


#undef __CC_SUPPORTS_WARNING
#include <sys/syslimits.h>	/* PATH_MAX */
/*
**
** Load traced proccess' elf and its depends.
**
*/
void	load_elfobjects(char* bin)
{
  char			buf[PATH_MAX + 1];
  struct link_map	map;
  Elf_Addr		addr;

  if (!elfobjects)
    {
      openelf(bin);
      if (elfobjects)
	elfobjects->base = 0;
    }
  /*
  ** Get the link map of the traced process and load depends.
  */
  for (addr = get_link_map_list(elfobjects); addr; addr = (Elf_Addr) map.l_next)
    {
      if (readmem_frm_exec(addr, sizeof(struct link_map), (char*) &map) == -1)
	break;
      if ((Elf_Addr) map.l_addr <= elfobjects->header->e_entry)
	continue;
      if (map.l_name == 0)
	continue;
      if (readmem_frm_exec((long) map.l_name, PATH_MAX, (char*) buf) == -1)
	continue;
      openelf(buf);
      elfobjects->base = (Elf_Addr) map.l_addr;
    }
  printf("\n");
}

/*
**
** Find the elf object within which address lies.
**
*/
elfobj*	find_elfobj(Elf_Addr addr)
{
  elfobj*	cur;
  Elf_Addr	seg;
  int		i;

  for (cur = elfobjects; cur; cur = cur->next)
    {
      for (i = 0; i < cur->header->e_phnum; i++)
	{
	  seg = cur->program_headers[i]->p_vaddr + cur->base;
	  if (addr >= seg && addr < seg + cur->program_headers[i]->p_memsz)
	    return (cur);
	}
    }
  return (NULL);
}

/*
**
** Get section from name.
**
*/
Elf_Shdr*	get_elfsection(elfobj* obj, char* name)
{
  int			i;
  Elf_Shdr*		ret = NULL;

  for (i = 0; i < obj->header->e_shnum; i++)
    if (!strcmp(obj->section_headers[i]->sh_name + obj->section_str, name))
      ret = obj->section_headers[i];
  return (ret);
}

/*
**
** Get symbol from section.
**
*/
Elf_Sym*		get_elfsym_frm_section(elfobj* obj, Elf_Addr addr, Elf_Shdr* section, int type)
{
  Elf_Sym*	start, * end, * ret = NULL;
  int		stop = 0;

  start = (Elf_Sym *) ((char*) obj->header + section->sh_offset);
  end   = (Elf_Sym *) ((char*) obj->header + section->sh_offset + section->sh_size);

  /*
  ** Try to find a symbol whose virtual range includes the target address
  ** else take a symbol with the highest address less than or equal to the
  ** target
  */
  for (; !stop && start < end; start++)
    {
      /*
      ** SHF_ALLOC from the man: If the file has a loadable segment that includes
      ** the symbol table, the section's attributes will include the SHF_ALLOC bit;
      ** otherwise, that bit will be off.
      */
      if (!(
	    (type == STT_NOTYPE || ELF_ST_TYPE(start->st_info) == type) &&
	    start->st_value <= addr &&
	    (obj->section_headers[start->st_shndx]->sh_flags & SHF_ALLOC))
	  )
	continue;

      if (start->st_size)
	{
	  if (start->st_size + start->st_value > addr)
	    {
	      ret = start;
	      stop = 1;
	    }
	}
      else
	{
	  if (ret == NULL || ret->st_value < start->st_value)
	    ret = start;
	}
    }
  return (ret);
}

/*
**
** Get the symbol's string.
**
*/
char*	get_symstr(elfobj* obj, Elf_Shdr* section, Elf_Sym* sym)
{
  char*	str;

  str = (char*) ((char*)obj->header + obj->section_headers[section->sh_link]->sh_offset);
  return (str);
}

/*
**
** Get the symbol that is associated to an address.
**
*/
int	get_elfsym(Elf_Addr addr, Elf_Sym** sym, char** symstr)
{
  char*		sections[] = { ".symtab", ".dynsym" , NULL };
  int		i;
  Elf_Shdr*	section;
  elfobj*	obj;

  *sym = NULL;
  obj = find_elfobj(addr);
  if (!obj)
    return (-1);
  for (i = 0; sections[i]; i++)
    {
      section = get_elfsection(obj, sections[i]);
      if (!section)
	continue;
      *sym = get_elfsym_frm_section(obj, addr - (Elf_Addr) obj->base, section, STT_FUNC);
      if (!*sym)
	continue;
      break;
    }
  if (!*sym)
    return (-1);
  *symstr = get_symstr(obj, section, *sym);
  return (0);
}

/*
**
** Get the string that is associated to an address.
**
*/
char*	get_elfsym_label(Elf_Addr addr)
{
  char*		symstr;
  Elf_Sym*	sym;

  if (get_elfsym(addr, &sym, &symstr) == -1)
    return (NULL);
  return (symstr + sym->st_name);
}

/*
**
** Get the symbol that is associated to a string.
**
*/
Elf_Sym*	get_elfsym_from_label(elfobj* obj, char* label)
{
  Elf_Shdr*	section;
  char*		sections[] = { ".symtab", ".dynsym" , NULL };
  char*		str;
  int		i;
  Elf_Sym*	start, * end;


  for (i = 0; sections[i]; i++)
    {
      section = get_elfsection(obj, sections[i]);
      if (!section)
	continue;

      str = obj->data + obj->section_headers[section->sh_link]->sh_offset;

      start = (Elf_Sym *) (obj->data + section->sh_offset);
      end = (Elf_Sym *) (obj->data + section->sh_offset + section->sh_size);
      for (; start < end; start++)
	{
	  if (!strcmp(str + start->st_name, label))
	    return (start);
	}
    }
  return (NULL);
}

/*
**
** Get the address from a symbol string.
**
*/
Elf_Addr	get_addr_from_label(char* label)
{
  elfobj*	cur;
  Elf_Sym*	ret = NULL;

  for (cur = elfobjects; cur; cur = cur->next)
    {
      ret = get_elfsym_from_label(cur, label);
      if (ret)
	return (ret->st_value + cur->base);
    }
  return (0);
}

/*
**
** Stabs Format.
** Get line number, file ... from the stab section.
** gcc -g
**
*/
#include <nlist.h>
#include <stab.h>
char*	get_debuginfo_from_addr(Elf_Addr addr)
{
  Elf_Addr	back;
  Elf_Shdr*	section, * section_str;
  elfobj*	obj;
  struct nlist*	start, * end;
  static char	buf[256];
  char		tmp[256];
  char*		str;
  int		linenbr = -1;


  obj = find_elfobj(addr);
  if (!obj)
    return (NULL);

  section = get_elfsection(obj, ".stab");
  if (!section)
    return (NULL);

  section_str = get_elfsection(obj, ".stabstr");
  if (!section_str)
    return (NULL);

  str = (char*) (obj->data + section_str->sh_offset);

  start = (struct nlist*) (obj->data + section->sh_offset);
  end   = (struct nlist*) (obj->data + section->sh_offset + section->sh_size);
  for (; start < end; start++)
    {
      if (start->n_type == N_FUN && start->n_value == addr)
	{
	  linenbr = start->n_desc;
	  break;
	}
      if (start->n_type == N_SO)
	{
	  if (back != start->n_value || !*buf)
	    {
	      back = start->n_value;
	      bzero(buf, sizeof(buf));
	    }
	  sprintf(tmp, "%s", str +  (int)start->n_name);
	  strcat(buf, tmp);
	}
    }
  if (*buf)
    {
      sprintf(tmp, "at %s:%d", buf, linenbr);
      sprintf(buf, "%s", tmp);
    }
  return (buf);
}

#include <sys/param.h>
#ifdef __NetBSD__
#define Elf_Note	Elf_Nhdr
#define roundup2	roundup
#endif /* __NetBSD__ */
/*
**
** Get the contain of a note.
**
*/
int	cpy_frm_elfnote(elfobj* obj, char* note_name, int note_type, void* buf, int len)
{
  int		i;
  Elf_Note*	note;
  char*		start, * end, * cur_name, * data;

  for (i = 0; i < obj->header->e_phnum; i++)
    {
      if (obj->program_headers[i]->p_type == PT_NOTE)
	{
	  start = obj->data + obj->program_headers[i]->p_offset;
	  end = start + obj->program_headers[i]->p_filesz;
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

/*
**
** Get the contain of a note in a core file.
**
*/
int	cpy_frm_core_elfnote(char* note_name, int note_type, void* buf, int len)
{
  if (!elfcore)
    return (-1);
  return (cpy_frm_elfnote(elfcore, note_name, note_type, buf, len));
}

/*
**
** Read from the traced process.
** If the process is running -> ptrace
** If the process is a core see after.
**
*/
int	readmem_frm_exec(long addr, int len, char* laddr)
{
  int		i, ui, r = 0;
  char*		data;
  elfobj*	obj;

  if (!elfcore)
    return (copy_from_son(addr, len, laddr));
  /*
  ** Read from core
  */
  obj = elfcore;
  while (len)
    {
      for (i = 0; i < obj->header->e_phnum; i++)
	{
	  if (obj->program_headers[i]->p_type == PT_LOAD &&
	      obj->program_headers[i]->p_vaddr <= addr &&
	      obj->program_headers[i]->p_vaddr + obj->program_headers[i]->p_memsz > addr)
	    break;
	}
      if (i < obj->header->e_phnum)
	{
	  ui = MIN(obj->program_headers[i]->p_vaddr + obj->program_headers[i]->p_memsz - addr, len);
	  data = obj->data + obj->program_headers[i]->p_offset + addr - obj->program_headers[i]->p_vaddr;
	  memcpy((char *) laddr + r, data, ui);
	  len -= ui;
	  r += ui;
	}
      else
	  return (r);
    }
  return (r);
}

#ifdef __NetBSD__
#include <sys/types.h>
#include <sys/ptrace.h>
#endif /* __NetBSD__ */
#ifdef __FreeBSD__
#include <sys/procfs.h>
#endif /* __FreeBSD__ */
/*
**
** Get registers from the traced process.
** If the process is running -> ptrace.
** If the process is a core -> note.
**
*/
struct reg	elf_get_registers(void)
{
  struct reg	cur_reg;

#ifdef __FreeBSD__
  prstatus_t	prstatus;

  if (cpy_frm_core_elfnote("FreeBSD", NT_PRSTATUS, &prstatus, sizeof(prstatus_t)) == -1)
    errexit("backtrace: Can't read registers from core !");
  cur_reg = prstatus.pr_reg;
#endif /* __FreeBSD__ */
#ifdef __NetBSD__
  if (cpy_frm_core_elfnote("NetBSD-CORE@1", PT_GETREGS, &cur_reg, sizeof(struct reg)) == -1)
    errexit("backtrace: Can't read registers from core !");
#endif /* __NetBSD__ */
  return (cur_reg);
}


#ifdef __NetBSD__
#include <sys/exec_elf.h>
#endif /* __NetBSD__ */
/*
**
** Get the bin name which have produced the core.
**
*/
char*	get_image_name_frm_core(void)
{
#ifdef __NetBSD__
  static struct netbsd_elfcore_procinfo	procinfo;

  if (cpy_frm_core_elfnote("NetBSD-CORE", ELF_NOTE_NETBSD_CORE_PROCINFO, &procinfo,
  sizeof(struct netbsd_elfcore_procinfo)) == -1)
    errexit("backtrace: Can't read bin image name from core !");
  return (procinfo.cpi_name);
#endif /* __NetBSD__ */
  return (NULL);
}
