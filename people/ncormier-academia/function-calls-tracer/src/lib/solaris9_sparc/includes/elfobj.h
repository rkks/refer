/*
** elfobj.h for  in /home/nico/lang/c/ftrace/src/lib/freebsd_ia32/bin/includes
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Sun Mar 26 19:22:30 2006 nicolas
** Last update Tue Apr 25 21:38:00 2006 aurelien nephtali
*/

#ifndef __ELFOBJ_H__
#define __ELFOBJ_H__

#define ELFSIZE	32
#define IS_ELF(ehdr)    ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                         (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                         (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                         (ehdr).e_ident[EI_MAG3] == ELFMAG3)

#define MIN(X, Y)  ((X) < (Y) ? (X) : (Y))

#include <elf.h>

#define Elf_Addr	Elf32_Addr
#define Elf_Ehdr	Elf32_Ehdr
#define Elf_Shdr	Elf32_Shdr
#define Elf_Phdr	Elf32_Phdr
#define Elf_Sym		Elf32_Sym
#define ELF_ST_TYPE	ELF32_ST_TYPE
#define Elf_Dyn		Elf32_Dyn


struct elfobj_s
{
  Elf_Addr		base;
  Elf_Ehdr*		header;
  Elf_Shdr**		section_headers;
  Elf_Phdr**		program_headers;
  char*			section_str;
};
typedef struct elfobj_s	elfobj_t;


#endif /* __ELFOBJ_H__ */
