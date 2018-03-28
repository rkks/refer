/*
** high_layer.c for  in /home/nico/lang/c/ftrace/lib/freebsd_ia32/proc
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:58:21 2006 nicolas
** Last update Wed Apr 26 23:38:27 2006 nicolas cormier
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/link.h>
#include <limits.h>
#include <errno.h>

#include "elfobj.h"

#include "api/includes/prototypes/proc/high_layer.h"
#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/prototypes/prim_types.h"

#include "api/includes/types/prim_types.h"

#include "procfs.h"
#include "includes/error.h"

/*
** Include static procfs utils
*/
#define __DONT_USE_READ_PROCFS__
#include "procfs.c"
#undef __DONT_USE_READ_PROCFS__

/*
** If Succeed: Return A Copy Of Bin Path.
*/
char*	proc_get_bin_path(proc_obj_t* obj)
{
  char	buf[256];

  /*
  ** With procfs
  */
  if (sprintf(buf, "/proc/%d/object/a.out", (int) obj->pid) == -1)
    return (NULL);
  if (!access(buf, R_OK))
    return (strdup(buf));

  return (NULL);
}

list_t*	proc_get_depends_list(proc_obj_t* obj)
{
  Elf_Ehdr		ehdr;
  Elf_Phdr		phdr;
  Elf_Dyn		dyn;
  struct link_map	lmap;
  unsigned long		phdr_addr, dyn_addr, addr;
  struct r_debug	rdebug;
  char			buf[PATH_MAX + 1];
  list_t*		ret = NULL;
  depend_t*		cur = NULL;

  /*
  ** Get elf header.
  */
  if (proc_read(obj, 0x10000, sizeof(Elf32_Ehdr), (char*) &ehdr) == -1)
    return (NULL);
  if (!IS_ELF(ehdr))
    {
      errno = EFTRACE;
      ftrace_errstr = "Not Elf ?!";
      exit(1);
      return (NULL);
    }
  /*
  ** Get program Headers and find the Dynamic program header..
  */
  phdr_addr = 0x10000 + ehdr.e_phoff;
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

list_t*	proc_backtrace(proc_obj_t* obj)
{
  obj = NULL;
  return (NULL);
}

int	proc_set_breakpoint(proc_obj_t* obj, brkp_t* brkp)
{
  long		ctl[1 + sizeof (priovec_t) / sizeof (long) +        /* PCREAD */
		    1 + sizeof (priovec_t) / sizeof (long)];        /* PCWRITE */
  long*		ctlp = ctl;
  size_t	size;
  priovec_t*	iovp;
  addr_t	bpt = 0x91d02001;
  addr_t	old;


  /*
  ** Fetch the old instruction.
  */
  *ctlp++ = PCREAD;
  iovp = (priovec_t*) ctlp;
  iovp->pio_base = &old;
  iovp->pio_len = sizeof(old);
  iovp->pio_offset = brkp->addr;
  ctlp += sizeof (priovec_t) / sizeof (long);
  /*
  ** Write the BPT instruction.
  */
  *ctlp++ = PCWRITE;
  iovp = (priovec_t*) ctlp;
  iovp->pio_base = &bpt;
  iovp->pio_len = sizeof (bpt);
  iovp->pio_offset = brkp->addr;
  ctlp += sizeof (priovec_t) / sizeof (long);
  /*
  ** Exec.
  */
  size = (char*) ctlp - (char*) ctl;
  if (write_procfs(obj, "ctl", size, (char*) ctl, 0) != (int) size)
    return (-1);
  /*
  ** Check if there was already a breakpoint.
  */
#if 0
  if (old == 0x91d02001)
    {
      errno = EFTRACE;
      ftrace_errstr = "Process is already debugged by another process !";
      return (-1);
    }
#endif
  brkp->sinc = old;
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

static regs_t*	last_call_reg = NULL;
static void	detect_call_and_save_args(proc_obj_t* obj, regs_t* regs)
{
  addr_t	r;

  if (proc_read(obj, regs->greg[R_PC], sizeof(int), (char*) &r) != -1)
    {
      if ((0xFF000000 & r) == 0x7f000000 ||
	  (0xFF000000 & r) == 0x40000000)
	{
	  if (last_call_reg)
	    free(last_call_reg);
	  last_call_reg = proc_get_registers(obj);
	}
    }
}

static addr_t	last_fp = 0;
int	proc_seek_calls(proc_obj_t* obj, addr_t* ioaddr)
{
  regs_t*	regs = NULL;
  int		ret = 0;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);

  /*
  ** Detect if the current instruction is a call.
  ** Save current registers for proc_seek_{args, return}.
  */
  detect_call_and_save_args(obj, regs);

  *ioaddr = regs->greg[R_PC];
  if (last_fp > (ulong) regs->greg[R_FP])
    {
      last_fp = regs->greg[R_FP];
      ret = CALL;
    }
  else if (last_fp < (ulong) regs->greg[R_FP])
    {
      last_fp = regs->greg[R_FP];
      ret = RET;
    }
  free(regs);
  return (ret);
}

static void	long_to_strval(long l, arg_t* arg)
{
  switch (arg->size)
    {
    case sizeof(int):
      if (arg->type && strchr(arg->type, (int) '*'))
	{
	  if (sprintf(arg->value, "0x%x", (int) l) == -1)
	    arg->value[0] = 0;
	}
      else if (sprintf(arg->value, "%d", (int) l) == -1)
	arg->value[0] = 0;
      break;
    case sizeof(double):
      if (sprintf(arg->value, "%lg", (double) l) == -1)
	    arg->value[0] = 0;
      break;
    case sizeof(long double):
      /* if (sprintf(arg->value, "%Lg", l) == -1) */
	arg->value[0] = 0;
      break;
    }
}

int	proc_seek_args(proc_obj_t* obj, args_t* args)
{
  regs_t*	regs = NULL;
  list_t*	list = NULL;
  arg_t*	arg = NULL;
  int		i;
  addr_t	cur_addr = 0;

  if (!args)
    return (-1);

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);

  /*
  ** Browse the stack to find args.
  */
  for (list = args->prev, i = 0; list; list = list->prev, i++)
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

      /*
      ** 7 first arguments are in register R_O0 to R_06
      ** nexts are on the stack.
      */
      arg->value[0] = 0;
      if (last_call_reg && i < 6)
	long_to_strval(last_call_reg->greg[R_O0 + i], arg);
      else
	{
	  long	ulr;

	  if (cur_addr == 0)
	    cur_addr = regs->greg[R_FP] + 92;
	  if (proc_read(obj, cur_addr, sizeof(long), (char*) &ulr) != sizeof(long))
	    return (-1);
	  long_to_strval(ulr, arg);
	  cur_addr += arg->size;
	}

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
          if (sprintf(ret->value, "0x%x", regs->greg[R_O0]) == -1)
            {
              if (regs)
                free(regs);
              return (-1);
            }
        }
      else if (sprintf(ret->value, "%d", regs->greg[R_O0]) == -1)
        {
          if (regs)
            free(regs);
          return (-1);
        }
      break;
    case sizeof(double):
      /*       if (sprintf(arg->value, "%lg", *((double*) &r)) == -1) */
      /*      { */
      /*        if (regs) */
      /*          free(regs); */
      /*        return (-1); */
      /*      } */
      break;
    case sizeof(long double):
      /*       if (sprintf(arg->value, "%Lg", *((long double*) &r)) == -1) */
      /*      { */
      /*        if (regs) */
      /*          free(regs); */
      /*        return (-1); */
      /*      } */
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
      if ((char*) regs->greg[R_PC] == ((char*)((brkp_t*) cur->value)->addr))
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

/*   printf("fs: 0x%x\nes: 0x%x\nds: 0x%x\nedi: 0x%x\nesi: 0x%x\n" */
/* 	 "ebp: 0x%x\nisp: 0x%x\nebx: 0x%x\nedx: 0x%x\necx: 0x%x\n" */
/* 	 "eax: 0x%x\ntrapno: 0x%x\nerr: 0x%x\neip: 0x%x\n" */
/* 	 "cs:: 0x%x\neflags: 0x%x\nesp: 0x%x\nss: 0x%x\ngs: 0x%x\n", */
/* 	 regs->r_fs, regs->r_es, regs->r_ds, regs->r_edi, regs->r_esi, */
/* 	 regs->r_ebp, regs->r_isp, regs->r_ebx, regs->r_edx, regs->r_ecx, */
/* 	 regs->r_eax, regs->r_trapno, regs->r_err, regs->r_eip, regs->r_cs, */
/* 	 regs->r_eflags, regs->r_esp, regs->r_ss, regs->r_gs */
/* 	 ); */

  free(regs);
}

int	proc_get_dependload_addr(proc_obj_t* obj, addr_t* ret_addr)
{
  obj = NULL;
  ret_addr = NULL;
  return (-1);
}
