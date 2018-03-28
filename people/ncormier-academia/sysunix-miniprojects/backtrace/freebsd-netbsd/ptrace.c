/*
** ptrace.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Jan 16 14:56:08 2006 nicolas
** Last update Wed Jan 25 17:28:29 2006 nicolas cormier
*/

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "backtrace.h"
#include "elffprot.h"

caddr_t*	get_bkrpoints(void);
struct reg	get_registers(void);

extern int	son;
extern int	attach;
extern int	verbose;

/*
**
** Copy data from traced process space.
**
*/
int	copy_from_son(long addr, int len, char* laddr)
{
  int	ret = 0;
  int	m;
  long	val;

  if (addr & (sizeof(long) - 1))
    errexit("backtrace: align");
  while (len)
    {
      errno = 0;
      val = ptrace(PT_READ_D, son, (char *) addr, 0);
      if (errno)
	return (-1);
      m = MIN((int) sizeof(long), len);
      memcpy(laddr, (const void*) &val, m);
      addr += sizeof(long);
      laddr += m;
      len -= m;
      ret += m;
    }
  return (ret);
}

/*
**
** Get registers from traced process.
**
*/
struct reg	ptrace_get_registers(void)
{
  struct reg	cur_reg;

  errno = 0;
  ptrace(PT_GETREGS, son, (caddr_t) &cur_reg, 0);
  if (errno)
    errexit("backtrace: ptrace(PT_GETREGS), %s", strerror(errno));
  return (cur_reg);
}

static int*	brkcont = NULL;
static int	brklen = 0;

void	free_brkcont(void)
{
  if (brkcont)
    {
      free(brkcont);
      brkcont = NULL;
    }
}

/*
**
** Restore funcs.
**
*/
void	restore_data(int idx)
{
  caddr_t*	ptr;

  ptr = get_bkrpoints();

  errno = 0;
  ptrace(PT_WRITE_D, son, ptr[idx], brkcont[idx]);
  if (errno)
    errexit("backtrace: can't restore instruction: ptrace(PT_WRITE_D), %s", strerror(errno));
}

void	restore_alldata(void)
{
  int		i;
  caddr_t*	ptr;

  ptr = get_bkrpoints();
  for (i = 0; ptr && ptr[i]; i++)
    restore_data(i);
}

void	restore_bkrpoint(int idx)
{
  caddr_t*	ptr;

  ptr = get_bkrpoints();

  errno = 0;
  ptrace(PT_WRITE_D, son, ptr[idx], 0xCC);
  if (errno)
    errexit("backtrace: can't restore instruction: ptrace(PT_WRITE_D), %s", strerror(errno));
}

extern int	backtrace_main;
/*
**
** Try to get addr from label
** if finded add a breakpoint on it.
**
*/
void   solve_human_brkpoints(void)
{
  char**	hbrks;
  caddr_t	addr;

  for (hbrks = get_humanbrkps(); hbrks && *hbrks; hbrks++)
    if ((addr = (caddr_t) get_addr_from_label(*hbrks)))
      {
	add_rbrkpoints(addr);
	/*
	** Always have a breakpoint on main,
	** if user have really set a brk on main backtrace_main = 1
	*/
	if (!strcmp(*hbrks, "main"))
	  backtrace_main = 1;
      }
}

static int	depend_load = 0;
/*
**
** Inject breakpoints in traced process.
**
*/
void	inject_brkpoints(void)
{
  caddr_t*	ptr;
  int		i = 0, ret;

  /*
  ** Inject Main brkp if traced proc is a son.
  */
  if (!brkcont)
    {
      load_elfobjects(get_bin_path(R_OK));
      if (attach)
	{
	  depend_load = 1;
	  solve_human_brkpoints();
	}
      else
	add_rbrkpoints((caddr_t) (get_addr_from_label("main") + 6));
    }

  ptr = get_bkrpoints();
  if (!ptr)
    return;
  if (!brkcont)
    {
      if (!(brkcont = malloc((brklen + 1) * sizeof(int))))
	errexit("backtrace: malloc, %s", strerror(errno));
      brklen++;
    }
  else
    {
      if (!(brkcont = realloc(brkcont, (brklen + 1) * sizeof(int))))
	errexit("backtrace: realloc, %s", strerror(errno));
      brklen++;
    }
  for (i = 0, ptr = get_bkrpoints(); ptr[i]; i++)
    {
      errno = 0;
      ret = ptrace(PT_READ_D, son, ptr[i], 0);
      if (ret == 0xCC || errno)
	continue;
      brkcont[i] = ret;
      errno = 0;
      ptrace(PT_WRITE_D, son, ptr[i], 0xCC);
      if (errno)
	continue;
      if (verbose)
	printf("Add Breakpoint on 0x%x, on the value: 0x%x\n", (unsigned int) ptr[i], brkcont[i]);
    }
}

static int	restore = -1;
static int	step = 0;

/*
**
** If sigtrap is due to a breakpoint
**
** 1) restore real instruction, go one step back
** 2) after exec of the real instruction, do a backtrace
** 3) wait step
** 4) re-inject the breakpoint and continue execution.
**
*/
int	chk_brkpoints(void)
{
  struct reg	cur_reg;
  caddr_t*	ptr;
  int		i;

  cur_reg = get_registers();
  if (!(ptr = get_bkrpoints()))
    return (0);

  for (i = 0; ptr[i]; i++)
    {
      /*
      ** Wait, reset & continue.
      */
      if (i == restore && ptr[i] != ((char*)cur_reg.r_eip))
	{
	  if (step == 1)
	    backtrace();
	  step++;
	  /*
	  ** Wait for 2 steps.
	  */
	  if (step <= 2)
	    break;
	  else
	    step = 0;
	  /*
	  ** Reset breakpoint & continue.
	  */
	  restore_bkrpoint(i);
	  restore = -1;
	  errno = 0;
	  ptrace(PT_CONTINUE, son, (caddr_t) 1, 0);
	  if (errno)
	    errexit("backtrace: ptrace(PTRACE_CONTINUE), %s", strerror(errno));
	  return (1);
	}
      /*
      ** If eip == breakpoint
      ** restore data and step back.
      */
      else if (ptr[i] == ((char*)cur_reg.r_eip) - 1)
	{
	  /*
	  ** Eip == main, load depends.
	  ** inject symbols break points.
	  */
	  if (!ptr[i + 1] && !depend_load)
	    {
	      load_elfobjects("");
	      depend_load = 1;
	      solve_human_brkpoints();
	      inject_brkpoints();
	    }
	  /*
	  ** Restore data on 0xCC
	  */
	  restore_data(i);
	  restore = i;
	  cur_reg.r_eip = (unsigned int) ptr[i];
	  /* printf("replace le data et on retourne a 0x%x\n", cur_reg.r_eip); */
	  errno = 0;
	  ptrace(PT_SETREGS, son, (caddr_t) &cur_reg, 0);
	  if (errno)
	    errexit("backtrace: ptrace(PTRACE_SETREGS), %s", strerror(errno));
	  break;
	}
    }

  if (!ptr[i])
    return (0);

  errno = 0;
  ptrace(PT_STEP, son, (caddr_t) 1, 0);
  if (errno)
    errexit("backtrace: ptrace(PTRACE_STEP), %s", strerror(errno));
  return (1);
}
