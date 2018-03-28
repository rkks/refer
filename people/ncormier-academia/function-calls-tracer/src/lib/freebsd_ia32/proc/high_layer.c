/*
** high_layer.c for  in /home/nico/lang/c/ftrace/lib/freebsd_ia32/proc
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:58:21 2006 nicolas
** Last update Thu Apr 27 15:59:16 2006 nicolas
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <elf.h>
#include <sys/link_elf.h>
#undef __CC_SUPPORTS_WARNING
#include <sys/syslimits.h>  /* PATH_MAX */
#include <errno.h>

#include "api/includes/prototypes/proc/high_layer.h"
#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/bin/high_layer/symbols.h"
#include "api/includes/prototypes/bin/low_layer.h"
#include "api/includes/prototypes/prim_types.h"

#include "api/includes/types/prim_types.h"
#include "api/includes/types/bin.h"
#include "api/includes/types/backtrace.h"
#include "api/includes/types/depend.h"

#include "includes/error.h"

#include "elfobj.h"
#include "proc.h"

/*
** If Succeed: Return A Copy Of Bin Path.
*/
char*	proc_get_bin_path(proc_obj_t* obj)
{
  char	buf[256];

  /*
  ** With procfs
  */
  if (sprintf(buf, "/proc/%d/file", obj->pid) == -1)
    return (NULL);
  if (!access(buf, R_OK))
    return (strdup(buf));

  return (NULL);
}

/*
** Restrieve link map from tracee.
** On NetBSD binary format is the ELF.
*/
list_t*	proc_get_depends_list(proc_obj_t* obj)
{
  Elf32_Ehdr		ehdr;
  Elf32_Phdr		phdr;
  Elf32_Dyn		dyn;
  /* Elf32_Word		got; */
  struct link_map	lmap;
  unsigned long		phdr_addr, dyn_addr, addr;
  /* unsigned long		map_addr; */
  struct r_debug		rdebug;
  char			buf[PATH_MAX + 1];
  list_t*		ret = NULL;
  depend_t*		cur = NULL;

  /*
  ** Get elf header.
  */
  if (proc_read(obj, 0x08048000, sizeof(Elf32_Ehdr), (char*) &ehdr) == -1)
    return (NULL);
  if (!IS_ELF(ehdr))
    {
      errno = EFTRACE;
      ftrace_errstr = "Not Elf ?!";
      return (NULL);
    }
  /*
  ** Get program Headers and find the Dynamic program header..
  */
  phdr_addr = 0x08048000 + ehdr.e_phoff;
  if (proc_read(obj, phdr_addr, sizeof(Elf32_Phdr), (char*) &phdr) == -1)
    return (NULL);
  while (phdr.p_type != PT_DYNAMIC)
    {
      if (proc_read(obj, phdr_addr += sizeof(Elf32_Phdr),
		    sizeof(Elf32_Phdr), (char*) &phdr) == -1)
	return (NULL);
    }
  if  (phdr.p_type != PT_DYNAMIC)
    return (NULL);
  /*
  ** Browse evrey dyn and find {DT_PLTGOT|DT_DEBUG}
  */
  if (proc_read(obj, phdr.p_vaddr, sizeof(Elf32_Dyn), (char*) &dyn) == -1)
    return (NULL);
  dyn_addr = phdr.p_vaddr;
  /*
  ** ---------------------------
  ** Retrieve with the got ..
  */
  /*
  while (dyn.d_tag != DT_PLTGOT)
    {
      if (proc_read(obj, dyn_addr += sizeof(Elf32_Dyn), sizeof(Elf32_Dyn), &dyn) == -1)
	return (NULL);
    }
  got = (Elf32_Word) dyn.d_un.d_ptr;
  got += 4;
  if (proc_read(obj, (unsigned long) got, 4, &map_addr) == -1)
    return (NULL);
  printf("map addr => 0x%x\n", map_addr);
  if (proc_read(obj, map_addr, sizeof(struct link_map), &lmap) == -1)
    return (NULL);
  */
  /*
  ** ---------------------------
  */
  /*
  ** ---------------------------
  ** Retrieve with the DT_DEBUG
  */
  while (dyn.d_tag != DT_DEBUG)
    {
      if (proc_read(obj, dyn_addr += sizeof(Elf32_Dyn), sizeof(Elf32_Dyn), (char*) &dyn) == -1)
	return (NULL);
    }
  if (dyn.d_tag != DT_DEBUG)
    return (NULL);
  /* printf("struct rdebug found @ 0x%x\n", dyn.d_un.d_ptr); */
  /*
  ** Get addresse's struct rdebug
  */
  if (proc_read(obj, dyn.d_un.d_ptr, sizeof(struct r_debug), (char*) &rdebug) == -1)
    return (NULL);
  /*
  ** Here we have the link_map addr.
  ** Create the depend list...
  */
  for (addr = (unsigned long) rdebug.r_map; addr; addr = (unsigned long) lmap.l_next)
    {
      if (proc_read(obj, addr, sizeof(struct link_map), (char*) &lmap) == -1)
	goto proc_get_depends_list_failed;
      if (lmap.l_name == 0)
	continue;
      if (proc_read(obj, (long) lmap.l_name, PATH_MAX, (char*) buf) == -1)
	goto proc_get_depends_list_failed;
      cur = malloc(sizeof(depend_t));
      if (cur == NULL)
	goto proc_get_depends_list_failed;
      cur->path = strdup(buf);
      cur->base_addr = (addr_t) lmap.l_addr;
      ret = list_add(ret, cur);
      if (ret == NULL)
	goto proc_get_depends_list_failed;
    }
  return (ret);
  /*
  ** If failed, free list And return NULL.
  */
 proc_get_depends_list_failed:
  for (; ret; )
    {
      cur = (depend_t*) ret->value;
      ret = list_del(ret, cur);
      if (cur && cur->path)
	free(cur->path);
      free(cur);
    }
  return (NULL);
}


/*
** Backtrace recursive function.
*/
static int	recur_stack_call_trace(proc_obj_t* obj, regs_t* cur_reg, int count, list_t** listret)
{
  int		ret;
  caddr_t	cur = (caddr_t) cur_reg->r_ebp;
  caddr_t	eip, ebp;
  backt_t*	backt;

  eip = (caddr_t) cur_reg->r_eip;
  ebp = (caddr_t) cur_reg->r_ebp;

  if (proc_read(obj, (long) cur, sizeof(int), (char*) &ret) == -1)
    return (-1);

  cur_reg->r_ebp = ret;
  if ((caddr_t) ret <= ebp)
    return (0);

  if (proc_read(obj, (long)(cur + sizeof(int)), sizeof(int), (char*) &ret) == -1)
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

list_t*	proc_backtrace(proc_obj_t* obj)
{
  list_t*	ret = NULL;
  regs_t*	cur_reg = NULL;
  backt_t*	backt;

  cur_reg = proc_get_registers(obj);
  if (cur_reg == NULL)
    goto proc_backtrace_failed;

  backt = malloc(sizeof(backt_t));
  if (backt == NULL)
    goto proc_backtrace_failed;

  backt->addr = (addr_t) cur_reg->r_eip;
  backt->count = 0;

  if (recur_stack_call_trace(obj, cur_reg, 1, &ret) == -1)
    goto proc_backtrace_failed;

  return (ret);

  /*
  ** If failed, free list And return NULL.
  */
 proc_backtrace_failed:
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

int	proc_set_breakpoint(proc_obj_t* obj, brkp_t* brkp)
{
  addr_t	buf = 0xCC;
  if (proc_read(obj, brkp->addr, sizeof(addr_t), (char*) &(brkp->sinc)) == -1)
    return (-1);
  if (proc_write(obj, brkp->addr, sizeof(addr_t), (char*) &buf) == -1)
    return (-1);
  brkp->set = 1;
  return (0);
}

int	proc_unset_breakpoint(proc_obj_t* obj, brkp_t* brkp)
{
  if (proc_write(obj, brkp->addr, sizeof(addr_t), (char*) &brkp->sinc) == -1)
    return (-1);
  brkp->set = 0;
  return (0);
}

/*
** Test if addr is in the rtld.
*/
static addr_t	rtld_low_addr = 0;
static addr_t	rtld_high_addr = 0;
static int	seek_rtld_addresses(proc_obj_t* obj)
{
  depends_t*	depends;
  list_t*	list;
  depend_t*	dep;
  int		ret = 0;

  depends = proc_get_depends_list(obj);
  for (list = depends; list; list = list->next)
    {
      dep = (depend_t*) list->value;
      if (dep && dep->path)
	{
	  if (!strcmp("/libexec/ld-elf.so.1", dep->path))
	    {
	      /*
	      ** Know we know where is loaded rtld.
	      */
	      bin_obj_t*	bin = NULL;
	      addr_t	seg, high = 0;
	      int	i;
	      elfobj_t*	luse;

	      bin = bin_new("/libexec/ld-elf.so.1", dep->base_addr);
	      if (bin == NULL)
		{
		  ret = -1;
		  break;
		}

	      luse = bin->luse;
	      for (i = 0; i < luse->header->e_phnum; i++)
		{
		  seg = luse->program_headers[i]->p_vaddr + luse->base;
		  if (high < seg + luse->program_headers[i]->p_memsz)
		    high = seg + luse->program_headers[i]->p_memsz;
		}
	      (void) bin_del(bin);
	      ret = 1;
	      rtld_low_addr = dep->base_addr;
	      rtld_high_addr = high;
	    }
	}
      if (list == depends->prev)
	break;
    }

  /*
  ** Free depends.
  */
  for (; depends; )
    {
      dep = (depend_t*) depends->value;
      depends = list_del(depends, dep);
      if (dep && dep->path)
	free(dep->path);
      free(dep);
    }
  return (ret);
}

static int	addr_is_in_rtld(proc_obj_t* obj, addr_t addr)
{
  int	r;

  if (!(rtld_high_addr && rtld_low_addr))
    {
      r = seek_rtld_addresses(obj);
      if (r == -1 || r == 0)
	return (r);
    }
  if (addr >= rtld_low_addr && addr <= rtld_high_addr)
    return (1);
  return (0);
}


#if 1

static addr_t	last_ebp = 0;
int	proc_seek_calls(proc_obj_t* obj, addr_t* ioaddr)
{
  regs_t*	regs = NULL;
  int		ret = 0;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);
  *ioaddr = regs->r_eip;
  if (last_ebp > regs->r_ebp)
    {
      last_ebp = regs->r_ebp;
      ret = CALL;
    }
  else if (last_ebp < regs->r_ebp)
    {
      last_ebp = regs->r_ebp;
      ret = RET;
    }
  if (addr_is_in_rtld(obj, regs->r_eip))
    ret = 0;
  free(regs);
  return (ret);
}

#else

# include "proc_seek_calls_no_frame.c"

#endif

int	proc_seek_args(proc_obj_t* obj, args_t* args)
{
  regs_t*	regs = NULL;
  list_t*	list = NULL;
  arg_t*	arg = NULL;
  addr_t	arg_addr;
  long double	r;

  if (!args)
    return (-1);

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);

  /*
  ** Browse the stack to find args.
  */
  arg_addr = regs->r_ebp + 8;
  for (list = args->prev; list; list = list->prev)
    {
      arg = (arg_t*) list->value;
      /*
      ** TODO struct ...
      */
      if (arg->size > sizeof(long double))
	{
	  if (regs)
	    free(regs);
	  return (-1);
	}
      if (arg->size < sizeof(int))
	arg->size = sizeof(int);
      if (proc_read(obj, arg_addr, sizeof(long double), (char*) &r) == -1)
	{
	  if (regs)
	    free(regs);
	  return (-1);
	}
      /*
      ** Value to string.
      */
      arg->value[0] = 0;
      switch (arg->size)
	{
	case sizeof(int):
	  if (arg->type && strchr(arg->type, (int) '*'))
	    {
	      if (sprintf(arg->value, "0x%x", *((int*) &r)) == -1)
		{
		  if (regs)
		    free(regs);
		  return (-1);
		}
	    }
 	  else if (sprintf(arg->value, "%d", *((int*) &r)) == -1)
	    {
	      if (regs)
		free(regs);
	      return (-1);
	    }
	  break;
	case sizeof(double):
 	  if (sprintf(arg->value, "%lg", *((double*) &r)) == -1)
	    {
	      if (regs)
		free(regs);
	      return (-1);
	    }
	  break;
	case sizeof(long double):
 	  if (sprintf(arg->value, "%Lg", *((long double*) &r)) == -1)
	    {
	      if (regs)
		free(regs);
	      return (-1);
	    }
	  break;
	}
      /*
      ** Move ptr to next arg in stack.
      */
      arg_addr += arg->size;
      if (list == args)
	break;
    }
  free(regs);
  return (0);
}

int	proc_seek_return_value(proc_obj_t* obj, arg_t* ret)
{
  regs_t*	regs = NULL;

  if (ret == NULL)
    return (-1);

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);

  if (sprintf(ret->value, "void") == -1)
    return (-1);

  switch (ret->size)
    {
    case sizeof(int):
    case sizeof(char):
    case sizeof(short):
      if (ret->type && strchr(ret->type, (int) '*'))
	{
	  if (sprintf(ret->value, "0x%x", regs->r_eax) == -1)
	    {
	      if (regs)
		free(regs);
	      return (-1);
	    }
	}
      else if (sprintf(ret->value, "%d", regs->r_eax) == -1)
	{
	  if (regs)
	    free(regs);
	  return (-1);
	}
      break;
    case sizeof(double):
/*       if (sprintf(arg->value, "%lg", *((double*) &r)) == -1) */
/* 	{ */
/* 	  if (regs) */
/* 	    free(regs); */
/* 	  return (-1); */
/* 	} */
      break;
    case sizeof(long double):
/*       if (sprintf(arg->value, "%Lg", *((long double*) &r)) == -1) */
/* 	{ */
/* 	  if (regs) */
/* 	    free(regs); */
/* 	  return (-1); */
/* 	} */
      break;
    }
  free(regs);
  return (1);
}

brkp_t*	proc_seek_brkps(proc_obj_t* obj, brkps_t* brkps)
{
  list_t*	cur;
  regs_t*	regs;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (NULL);

  for (cur = (list_t*) brkps; cur; cur = cur->next)
    {
      /*
      ** On x86-based machines, a breakpoint trap leaves the program
      ** counter (the EIP) referring to the breakpointed instruction
      ** plus one byte.
      */
      if ((char*) regs->r_eip == ((char*)((brkp_t*) cur->value)->addr) + 1)
	return ((brkp_t*) cur->value);
      if (cur == ((list_t*) brkps)->prev)
	break;
    }
  free(regs);
  return (NULL);
}

void	proc_dump_registers(proc_obj_t* obj)
{
  regs_t*	regs;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return;

  printf("fs: 0x%x\nes: 0x%x\nds: 0x%x\nedi: 0x%x\nesi: 0x%x\n"
	 "ebp: 0x%x\nisp: 0x%x\nebx: 0x%x\nedx: 0x%x\necx: 0x%x\n"
	 "eax: 0x%x\ntrapno: 0x%x\nerr: 0x%x\neip: 0x%x\n"
	 "cs:: 0x%x\neflags: 0x%x\nesp: 0x%x\nss: 0x%x\ngs: 0x%x\n",
	 regs->r_fs, regs->r_es, regs->r_ds, regs->r_edi, regs->r_esi,
	 regs->r_ebp, regs->r_isp, regs->r_ebx, regs->r_edx, regs->r_ecx,
	 regs->r_eax, regs->r_trapno, regs->r_err, regs->r_eip, regs->r_cs,
	 regs->r_eflags, regs->r_esp, regs->r_ss, regs->r_gs
	 );

  free(regs);
}

/*
** Get section from name.
*/
static Elf_Shdr*	get_elfsection(bin_obj_t* obj, char* name)
{
  int		i;
  Elf_Shdr*	ret = NULL;
  elfobj_t*	elfobj = NULL;

  elfobj = (elfobj_t*) obj->luse;
  for (i = 0; i < elfobj->header->e_shnum; i++)
    if (!strcmp(elfobj->section_headers[i]->sh_name + elfobj->section_str, name))
      ret = elfobj->section_headers[i];
  return (ret);
}

/*
** Get this:
** 0x43b0 SECTION .text
** 0x4a80 FUNCTION r_debug_state
*/
static int	get_rtld_infos(addr_t* text, addr_t* r_debug_state)
{
  bin_obj_t*	bin = NULL;
  syms_t*	syms = NULL;
  sym_t*	sym;
  int		ret = -1;
  Elf_Shdr*	section;


  bin = bin_new("/libexec/ld-elf.so.1", 0);
  if (bin == NULL)
    goto get_rtld_infos_failed;
  syms = syms_new(bin);
  if (syms == NULL)
    goto get_rtld_infos_failed;
  section = get_elfsection(bin, ".text");
  if (section == NULL)
    goto get_rtld_infos_failed;
  *text = section->sh_addr;
  sym = sym_get_frm_label(syms, "r_debug_state");
  if (sym == NULL)
    goto get_rtld_infos_failed;
  *r_debug_state = sym->addr;
  ret = 0;
 get_rtld_infos_failed:
  if (bin)
    bin_del(bin);
  if (syms)
    syms_del(syms);
  return (ret);
}

/*
** Get r_debug_state address.
** r_debug_state is call when every depends
** has been loaded.
*/
int	proc_get_dependload_addr(proc_obj_t* obj, addr_t* ret_addr)
{
  list_t*	list = NULL;
  depends_t*	depends = NULL;
  depend_t*	dep = NULL;
  int		base_addr_found = 0;
  addr_t	rtld_base_addr, text, r_debug_state;
  regs_t*	regs = NULL;

  /*
  ** Try to find rtld base address in loaded depends.
  */
  depends = proc_get_depends_list(obj);
  for (list = (list_t*) depends; list; list = list->next)
    {
      dep = (depend_t*) list->value;
      if (!strcmp(dep->path, "/libexec/ld-elf.so.1"))
	{
	  rtld_base_addr = dep->base_addr;
	  base_addr_found = 1;
	}
      if (list == ((list_t*) depends)->prev)
	break;
    }
  /*
  ** Free depends.
  */
  for (; depends; )
    {
      dep = (depend_t*) depends->value;
      depends = list_del(depends, dep);
      if (dep && dep->path)
	free(dep->path);
      free(dep);
    }
  /*
  ** If not found in depends,
  ** try to find with the first instruction start of .text.
  ** It should failed if we are between the first insctruction
  ** and the filling of the base rtld's depend in the linkmap.
  ** need address of .text and r_debug_state from rtld's binary.
  */
  if (get_rtld_infos(&text, &r_debug_state) == -1)
    return (-1);
  if (!base_addr_found)
    {
      regs = proc_get_registers(obj);
      if (regs)
	{
	  rtld_base_addr = regs->r_eip - text;
	  base_addr_found = 1;
	  free(regs);
	  regs = NULL;
	}
    }
  if (!base_addr_found)
    return (-1);
  /*
  ** Now we have the linker base address.
  */
  *ret_addr = rtld_base_addr + r_debug_state;
  return (0);
}
