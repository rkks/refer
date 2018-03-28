/*
** backtrace.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Wed Jan  4 14:34:36 2006 nicolas
** Last update Thu Jan 26 11:47:18 2006 nicolas cormier
*/

#include <sys/types.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include "backtrace.h"
#include "elffprot.h"

/*
**
** TODO
**
** option -v
**
*/

int	son = -1;
int	verbose = 0;
int	attach = 0;
char*	bin = NULL;
char*	core = NULL;

/*
**
** Get current registers
** from core file or from process.
**
*/
struct reg	get_registers(void)
{
  struct reg	cur_reg;

  if (!core)
    cur_reg = ptrace_get_registers();
  else
    cur_reg = elf_get_registers();
  return (cur_reg);
}


/*
**
** Try to find the traced proc image.
**
*/
char*	get_bin_path(int act)
{
  static char	buf[256];
  char*		wrd, * env, * img;

  if (bin && !access(bin, act))
    return (bin);

  if (core)
    {
      /*
      ** Try to get the bin name from the core
      ** works if the bin's path is '.' or is in PATH env.
      */
      if ((img = get_image_name_frm_core()))
	{
	  if (!access(bin, act))
	    return (bin);
	  bin = img;
	}
    }
  else
    {
      /*
      ** With procfs
      */
      if (sprintf(buf, "/proc/%d/file", son) == -1)
	errexit("backtrace: sprintf(\"/proc/%d/file\"), %s", strerror(errno));
      if (!access(buf, act))
	return (buf);
    }

  /*
  ** Try to find bin's path in the PATH env.
  */
  if (bin && (env = getenv("PATH")) )
    {
      for (wrd = strtok(env, ":"); wrd; wrd = strtok(NULL, ":"))
	{
	  if (sprintf(buf, "%s/%s", wrd, bin) == -1)
	    errexit("backtrace: sprintf(\"/proc/%d/file\"), %s", strerror(errno));
	  if (!access(buf, act))
	    return (buf);
	}
    }

  /* errexit("backtrace: unable to read executable '%s'", bin); */
  return (bin);
}

void	print_addr_info(caddr_t addr, int count)
{
  char*	str;

  str = get_elfsym_label((Elf_Addr) addr);
  printf("#%d %p %s", count, addr, str);
  if (str)
    str = get_debuginfo_from_addr(get_addr_from_label(str));
  if (str)
    printf("() %s", str);
  printf("\n");
}

int	backtrace_main = 0;

/*
** Always have a breakpoint on main,
** if user have really set a brk on main backtrace_main = 1
** else print nothing.
*/
void	backtrace(void)
{
  struct reg	cur_reg;
  char*		str;

  cur_reg = get_registers();

  str = get_elfsym_label(cur_reg.r_eip);
  if (str && !strcmp(str, "main") && !backtrace_main)
      return;

  printf("\n");
  print_addr_info((caddr_t) cur_reg.r_eip, 0);
  recur_stack_call_trace(&cur_reg, 1);
  printf("\n");

}

/*
**
** Trace core.
**
*/
static void	tracecore(void)
{
  load_elfcore(core);
  load_elfobjects(get_bin_path(R_OK));
  backtrace();
}

/*
**
** Trace running process.
**
*/
static void	trap_tracing(int t)
{
  if (kill(son, SIGTRAP) == -1)
    errexit("backtrace: kill(SIGTRAP), %s", strerror(errno));
}

static int	stop = 0;

/*
**
** Backtrace Loop.
** Wait for Breakpoints or SIGINT -> backtrace
**                         SIGQUIT -> exit backtrace
**
*/
static void	backtrace_loop(void)
{
  struct sigaction	action;
  int			ret, status, signal;

  action.sa_handler = trap_tracing;
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &action, NULL))
    errexit("backtrace: sigaction(SIGINT), %s", strerror(errno));
  load_elfcore(core);

  while (1)
    {
      ret = wait4(son, &status, 0, NULL);
      if (ret == -1)
	errexit("backtrace: wait4, %s", strerror(errno));

      if(WIFSTOPPED(status))
	{
	  signal = WSTOPSIG(status);
	  switch(signal)
	    {
	    case SIGTRAP:
	      /*
	      ** Check if user have stopped backtrace.
	      */
	      if (stop == 1)
		exit(0);
	      /*
	      ** Check if is a breakpoint
	      ** if true -> chk_brkpoints (ptrace.c)
	      ** else backtrace.
	      */
	      if ((ret = chk_brkpoints()))
		break;
	      backtrace();
	      errno = 0;
	      ptrace(PT_CONTINUE, son, (caddr_t) 1, 0);
	      if (errno)
		errexit("backtrace: ptrace(PTRACE_CONTINUE), %s", strerror(errno));
	      break;

	    default:
	      errno = 0;
	      printf("Program received signal %d, %s\n", signal, sys_siglist[signal]);
	      ptrace(PT_CONTINUE, son, (caddr_t) 1, signal);
	      if (errno)
		errexit("backtrace: ptrace(PTRACE_CONTINUE), %s", strerror(errno));
	    }
	}
      else if (WIFEXITED(status))
	{
	  printf("\nProgram exited normally.\n");
	  break;
	}
      else if (WIFSIGNALED(status))
	{
	  printf("Program exited with signal %d, %s\n", signal, sys_siglist[signal]);
	  break;
	}

    }
}

/*
**
** Free every malloc.
** If traced process has been attached, detach before quit.
**
*/
static void	clean_backtrace(void)
{
  if (attach)
    {
      /*
      ** Restore data instead of breakpoints.
      */
      restore_alldata();
      errno = 0;
      ptrace(PT_DETACH, son, (char *) 1, 0);
      if (errno)
	errexit("backtrace: ptrace(PT_DETACH), %s", strerror(errno));
    }
  free_elfobjects();
  free_brkpoints();
  free_brkcont();
}

static void    quit_backtrace(int t)
{
  stop = 1;
  trap_tracing(1);
}

int	main(int argc, char* argv[])
{
  struct sigaction	action;
  int			ch, i, count = 0;
  int			passed = 0;

  action.sa_handler = quit_backtrace;
  action.sa_flags = SA_RESTART;
  if (sigaction(SIGQUIT, &action, NULL))
    errexit("backtrace: sigaction(SIGQUIT), %s", strerror(errno));

  for (i = 0; i < argc; i++)
    if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "-e") || !strcmp(argv[i], "-c"))
      {
	if (i != argc - 1)
	  argc = i + 2;
	break;
      }
  if (atexit(clean_backtrace) == -1)
    errexit("backtrace: atexit:, %s", strerror(errno));
  for (count = 0; (ch = getopt(argc, argv,"vhb:p:e:c:")) != EOF; count++)
    {
       switch ((char) ch)
	{
	case 'b':
	  add_brkpoints(optarg);
	  break;
	case 'p':
	  passed = 1;
	  bin = *(argv + i + 2);
	  tracepid(optarg);
	  break;
	case 'e':
	  passed = 1;
	  traceson(optarg, argv + i + 1);
	  break;
	case 'c':
	  passed = 1;
	  core = optarg;
	  bin = *(argv + i + 2);
	  break;
	case 'v':
	  verbose = 1;
	  break;
	case 'h':
	  errexit(USAGE);
	  break;
	default:
	  errexit(USAGE);
	  break;
	}
      if (passed == 1)
	break;
    }

  if (!passed)
    errexit(USAGE);

  if (!core)
    backtrace_loop();
  else
    tracecore();

  return (0);
}
