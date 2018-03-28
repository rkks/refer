/*
** elffprot.h for  in /u/ept3/cormie_n/lang/c/backtrace
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Jan 20 11:28:32 2006 nicolas cormier
** Last update Sat Jan 21 16:55:41 2006 nicolas cormier
*/

#ifndef __ELFFPROC_H__
#define __ELFFPROC_H__

#ifdef __NetBSD__
#define ELFSIZE	32
#define IS_ELF(ehdr)    ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                         (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                         (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                         (ehdr).e_ident[EI_MAG3] == ELFMAG3)
#endif /* __NetBSD__ */

#include <machine/reg.h>

#include <elf.h>

struct s_elfobj
{
  struct s_elfobj*	next;
  void*			data;
  Elf_Addr		base;
  Elf_Ehdr*		header;
  Elf_Shdr**		section_headers;
  Elf_Phdr**		program_headers;
  int			size;
  char*			section_str;
  char			name[256];
};

typedef struct s_elfobj	elfobj;


elfobj*		get_elfobj_list(void);
void		fill_elfobj(char* path, void* data, unsigned int size);
void		openelf(char* path);
Elf_Addr	get_link_map_list(elfobj* bin);
void		load_elfcore(char* core);
void		load_elfobjects(char* bin);
elfobj*		find_elfobj(Elf_Addr addr);
Elf_Shdr*	get_elfsection(elfobj* obj, char* name);
Elf_Sym*	get_elfsym_frm_section(elfobj* obj, Elf_Addr addr, Elf_Shdr* section, int type);
char*		get_symstr(elfobj* obj, Elf_Shdr* section, Elf_Sym* sym);
int		get_elfsym(Elf_Addr addr, Elf_Sym** sym, char** symstr);
char*		get_elfsym_label(Elf_Addr addr);
Elf_Sym*	get_elfsym_from_label(elfobj* obj, char* label);
Elf_Addr	get_addr_from_label(char* label);
char*		get_debuginfo_from_addr(Elf_Addr addr);
int		cpy_frm_elfnote(elfobj* obj, char* note_name, int note_type, void* buf, int len);
int		cpy_frm_core_elfnote(char* note_name, int note_type, void* buf, int len);
int		readmem_frm_exec(long addr, int len, char* laddr);
struct reg	elf_get_registers(void);
char*		get_image_name_frm_core(void);
void		free_elfobjects(void);

#endif /* __ELFFPROC_H__ */
