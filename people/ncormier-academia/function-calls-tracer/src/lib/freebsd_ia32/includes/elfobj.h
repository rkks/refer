/*
** elfobj.h for  in /home/nico/lang/c/ftrace/src/lib/freebsd_ia32/bin/includes
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Sun Mar 26 19:22:30 2006 nicolas
** Last update Wed Apr  5 19:46:12 2006 nicolas cormier
*/

#ifndef __ELFOBJ_H__
#define __ELFOBJ_H__

#include <elf.h>

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
