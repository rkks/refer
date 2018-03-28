/*
** low_layer.c for  in /home/nico/lang/c/ftrace/src/api/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:03:53 2006 nicolas
** Last update Wed Apr 26 22:40:42 2006 nicolas cormier
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

#define _STRUCTURED_PROC	1
#include <sys/procfs.h>
#undef _STRUCTURED_PROC

#include "api/includes/prototypes/proc/low_layer.h"
#include "api/includes/types/regs.h"

#include "procfs.h"
#include "includes/error.h"

/*
** Include static procfs utils
*/
#include "procfs.c"

proc_obj_t*	proc_new(void)
{
  proc_obj_t*	new = NULL;

  new  = malloc(sizeof(proc_obj_t));
  if (new == NULL)
    return (NULL);

  new->path = NULL;
  new->pid = -1;
  new->attach = -1;
  new->luse = malloc(sizeof(procfs_fd_t));
  if (new->luse == NULL)
    {
      free(new);
      return (NULL);
    }
  ((procfs_fd_t*) new->luse)->ctl = -1;
  ((procfs_fd_t*) new->luse)->status = -1;
  ((procfs_fd_t*) new->luse)->ras = -1;
  ((procfs_fd_t*) new->luse)->was = -1;
  ((procfs_fd_t*) new->luse)->gwin = -1;
  return (new);
}

int	proc_del(proc_obj_t* obj)
{
  /*
  ** close fds
  */
  close(((procfs_fd_t*) obj->luse)->ctl);
  ((procfs_fd_t*) obj->luse)->ctl = -1;
  close(((procfs_fd_t*) obj->luse)->status);
  ((procfs_fd_t*) obj->luse)->status = -1;
  close(((procfs_fd_t*) obj->luse)->ras);
  ((procfs_fd_t*) obj->luse)->ras = -1;
  close(((procfs_fd_t*) obj->luse)->was);
  ((procfs_fd_t*) obj->luse)->was = -1;
  close(((procfs_fd_t*) obj->luse)->gwin);
  ((procfs_fd_t*) obj->luse)->gwin = -1;

  if (obj->luse)
    free(obj->luse);
  free(obj);
  return (0);
}

regs_t*	proc_get_registers(proc_obj_t* obj)
{
  regs_t*	ret;
  pstatus_t	ps;
#if 0
  gwindows_t	gw;
  int		i;
#endif

  if (read_procfs(obj, "status", sizeof(pstatus_t), (char*) &ps, 0) != sizeof(pstatus_t))
    return (NULL);

  ret = malloc(sizeof(regs_t));
  if (ret == NULL)
    return (NULL);

  (void) memcpy((char*) &(ret->greg), ps.pr_lwp.pr_reg, sizeof(prgregset_t));

  ret->has_gwin = 0;

#if 0
  /*
  ** TODO
  **
  ** We trat only main thread (lwp 1).
  ** Multi-threaded process should not work.
  */

  if (read_procfs(obj, "lwp/1/gwindows", sizeof(gwindows_t), (char*) &gw, 0) <= 0)
    return (ret);

  /*
  ** If we actually found a non-zero gwindows file and
  ** were able to read it, iterate through the buffers
  ** looking for a stack pointer match; if one is found,
  ** copy out the corresponding register window.
  */
  (void) memcpy((char*) &(ret->gwin), (char*) &gw, sizeof(gwindows_t));
#endif

  return (ret);
}

int	proc_set_registers(proc_obj_t* obj, regs_t* registers)
{
  writeregs_procfs_t	wp;

  wp.func = PCSREG;
  memcpy(&(wp.regs), registers->greg, sizeof(prgregset_t));

  if (write_procfs(obj, "ctl", sizeof(writeregs_procfs_t), (char*) &wp, 0)
      != sizeof(writeregs_procfs_t))
    return (-1);

  return (0);
}

int	proc_read(proc_obj_t* obj, addr_t raddr, unsigned int len, char* laddr)
{
  return (read_procfs(obj, "as", (size_t) len,  laddr, (off_t) raddr));
}

/*
** Must be align with a long!
*/
int	proc_write(proc_obj_t* obj, addr_t raddr, unsigned int len, char* laddr)
{
  return (write_procfs(obj, "as", (size_t) len,  laddr, (off_t) raddr));
}

int	proc_continue(proc_obj_t* obj, int signal)
{
  long		ctl[1 +                                     /* PCCFAULT     */
		    1 + sizeof (siginfo_t)/sizeof (long) +  /* PCSSIG/PCCSIG */
		    2 ];                                    /* PCRUN        */
  long*		ctlp = ctl;
  size_t	size;

  /*
  ** Clear fault && sig.
  */
  *ctlp++ = PCCFAULT;
  *ctlp++ = PCCSIG;
  /*
  ** Add sig.
  */
  if (signal)
    {
      siginfo_t *infop;

      infop = (siginfo_t*) ctlp;
      (void) memset(infop, 0, sizeof (*infop));
      infop->si_signo = signal;
      ctlp += sizeof (siginfo_t) / sizeof (long);
    }

  *ctlp++ = PCRUN;
  *ctlp++ = PRCSIG|PRCFAULT;
  size = (char*) ctlp - (char*) ctl;

  if (write_procfs(obj, "ctl", size, (char*) ctl, 0) == -1)
    return (-1);
  return (0);
}

int	proc_continue_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  regs_t*	regs;
  addr_t*	t;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);
  t = (addr_t*) &(regs->greg[R_PC]);
  *t = addr;
  if (proc_set_registers(obj, regs) == -1)
    {
      free(regs);
      return (-1);
    }
  free(regs);
  if (proc_continue(obj, signal) == -1)
    return (-1);
  return (0);
}

int	proc_step(proc_obj_t* obj, int signal)
{
  long		ctl[1 +                                     /* PCCFAULT     */
		    1 + sizeof (siginfo_t)/sizeof (long) +  /* PCSSIG/PCCSIG */
		    2 ];                                    /* PCRUN        */
  long*		ctlp = ctl;
  size_t	size;

  /*
  ** Clear fault && sig.
  */
  *ctlp++ = PCCFAULT;
  *ctlp++ = PCCSIG;
  /*
  ** Add sig.
  */
  if (signal)
    {
      siginfo_t *infop;

      infop = (siginfo_t*) ctlp;
      (void) memset(infop, 0, sizeof (*infop));
      infop->si_signo = signal;
      ctlp += sizeof (siginfo_t) / sizeof (long);
    }

  *ctlp++ = PCRUN;
  *ctlp++ = PRCFAULT|PRSTEP|PCCSIG;
  size = (char*) ctlp - (char*) ctl;

  if (write_procfs(obj, "ctl", size, (char*) ctl, 0) == -1)
    return (-1);
  return (0);
}

int	proc_step_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  regs_t*	regs;
  addr_t*	t;

  regs = proc_get_registers(obj);
  if (regs == NULL)
    return (-1);
  t = (addr_t*) &(regs->greg[R_PC]);
  *t = addr;
  if (proc_set_registers(obj, regs) == -1)
    {
      free(regs);
      return (-1);
    }
  free(regs);
  if (proc_step(obj, signal) == -1)
    return (-1);
  return (0);
}

int	proc_wait_debug_event(proc_obj_t* obj, int* sigret)
{
  long		ctl[2];
  pstatus_t	s_stat;

  while (1)
    {
      /*
      ** Wait for event.
      */
      ctl[0] = PCWSTOP;
      ctl[1] = 0;
      errno = 0;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1)
	{
	  if (errno != EINTR)
	    break;
	}
      /*
      ** Get status.
      */
      if (read_procfs(obj, "status", sizeof(pstatus_t), (char*) &s_stat, 0) == -1)
	break;
      if (s_stat.pr_lwp.pr_why == PR_SIGNALLED)
	{
	  *sigret = s_stat.pr_lwp.pr_cursig;
	  return (SIGNAL_EVENT);
	}
      return (DEBUG_EVENT);
    }
  if (errno == ENOENT)
    return (EXIT_EVENT);
  return (-1);
}

int	proc_wait_debug_event_nohang(proc_obj_t* obj, int* sigret)
{
  long		ctl[2];
  pstatus_t	s_stat;

  while (1)
    {
      /*
      ** Wait for event.
      */
      ctl[0] = PCTWSTOP;
      ctl[1] = 1;
      errno = 0;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1)
	{
	  if (errno != EINTR)
	    break;
	}
      /*
      ** Get status.
      */
      if (read_procfs(obj, "status", sizeof(pstatus_t), (char*) &s_stat, 0) == -1)
	break;
      if (s_stat.pr_lwp.pr_why == PR_SIGNALLED)
	{
	  *sigret = s_stat.pr_lwp.pr_cursig;
	  return (SIGNAL_EVENT);
	}
      return (DEBUG_EVENT);
    }
  if (errno == ENOENT)
    return (EXIT_EVENT);
  return (-1);
}

/*
** Tracer has just attached/created process.
** Wait until process is ready.
*/
static int      wait_for_tracee(proc_obj_t* obj)
{
  long		ctl[2];
  pstatus_t	s_stat;

  /*
  ** Waiting for execv.
  */
  while (1)
    {
      /*
      ** Wait for event.
      */
      ctl[0] = PCWSTOP;
      ctl[1] = 0;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1 && errno == EINTR)
	continue;
      /*
      ** Get status.
      */
      if (read_procfs(obj, "status", sizeof(pstatus_t), (char*) &s_stat, 0) == -1)
	break;
      if (s_stat.pr_lwp.pr_why == PR_SYSENTRY && IS_SYS_EXEC(s_stat.pr_lwp.pr_what))
	{
	  pcsexit_t	pcsexit;
	  /*
	  ** Define system calls notifications.
	  ** We want to know when son exit from exec.
	  */
	  pcsexit.ctl = PCSEXIT;
	  premptyset(&pcsexit.sysset);
	  praddset(&pcsexit.sysset, SYS_exec);
	  praddset(&pcsexit.sysset, SYS_execve);
	  if (write_procfs(obj, "ctl", sizeof(pcsentry_t), (char*) &pcsexit, 0) == -1)
	    return (-1);
	}
      else if (s_stat.pr_lwp.pr_why == PR_SYSEXIT && IS_SYS_EXEC(s_stat.pr_lwp.pr_what))
	{
	  /*
	  ** Execve Ok ! Ready to trace !
	  */
	  return (0);
	}
      ctl[0] = PCRUN;
      ctl[1] = 0;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
	break;
      /*
      ** Sleep a while.
      */
      (void) usleep(10);
    }
  errno = EFTRACE;
  ftrace_errstr = "Abnormal Process Exit";
  return (-1);
}


/*
** Create Process And Suspend it.
*/
int	proc_create(proc_obj_t* obj, char* path, char** argv)
{
  int	son;

  if (access(path, R_OK|X_OK))
    {
      errno = EFTRACE;
      ftrace_errstr = "You must have read permission on Solaris9/Sparc to trace a process.";
      return (-1);
    }

  if ((son = fork()) == -1)
    return (-1);
  /*
  ** Father exec,
  ** Waiting for tracee.
  */
  if (son)
    {
      long		ctl[2];
      pcsfault_t	pcsfault;
      pcsentry_t	pcsentry;


      /*
      ** Init proc obj.
      */
      obj->pid = son;
      obj->path = path;
      obj->attach = 0;
      /*
      ** Make sure that son is in pause().
      */
      (void) usleep(100);
      /*
      ** Let the child process stop first.
      */
      if (proc_stop(obj) == -1)
	return (-1);
      /*
      ** Set options.
      */
      ctl[0] = PCUNSET;
      ctl[1] = PR_ASYNC;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
	return (-1);
      ctl[0] = PCUNSET;
      ctl[1] = PR_FORK;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
	return (-1);
      /*
      ** Define a set of hardware faults to be traced in the process.
      ** FLTTRACE = single step
      ** FLTBPT =  breakpoint
      */
      pcsfault.ctl = PCSFAULT;
      premptyset(&pcsfault.s_fltset);
      praddset(&pcsfault.s_fltset, FLTTRACE);
      praddset(&pcsfault.s_fltset, FLTBPT);
      if (write_procfs(obj, "ctl", sizeof(pcsfault_t), (char*) &pcsfault, 0) == -1)
	return (-1);
      /*
      ** Define system calls notifications.
      ** We want to know when son calls exec.
      */
      pcsentry.ctl = PCSENTRY;
      premptyset(&pcsentry.sysset);
      praddset(&pcsentry.sysset, SYS_exec);
      praddset(&pcsentry.sysset, SYS_execve);
      if (write_procfs(obj, "ctl", sizeof(pcsentry_t), (char*) &pcsentry, 0) == -1)
	return (-1);
      /*
      ** Restart the son and abort the pause() call.
      */
      ctl[0] = PCRUN;
      ctl[1] = PRSABORT|PRCFAULT|PRCSIG/* |PRSTEP */;
      if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
	return (-1);
      /*
      ** Flush fds
      */
      close(((procfs_fd_t*) obj->luse)->ctl);
      ((procfs_fd_t*) obj->luse)->ctl = -1;
      close(((procfs_fd_t*) obj->luse)->status);
      ((procfs_fd_t*) obj->luse)->status = -1;
      close(((procfs_fd_t*) obj->luse)->ras);
      ((procfs_fd_t*) obj->luse)->ras = -1;
      close(((procfs_fd_t*) obj->luse)->was);
      ((procfs_fd_t*) obj->luse)->was = -1;
      /*
      ** Now waiting !
      */
      return (wait_for_tracee(obj));
    }
  /*
  ** Son exec
  */
  if (setpgid(0, 0) == -1)
    (void) exit(1);
  /*
  ** Waiting for PRSABORT from parent.
  */
  (void) pause();
  /*
  ** Execute the program.
  */
  (void) execv(path, argv);
  (void) exit(1);
  return (-1);
}

/*
** Attach Process And Suspend it.
*/
int	proc_attach(proc_obj_t* obj, pid_t pid)
{
  long		ctl[2];
  pcsfault_t	pcsfault;


  /*
  ** Init proc obj.
  */
  obj->pid = pid;
  obj->path = NULL;
  obj->attach = 1;
  /*
  ** Let the child process stop first.
  */
  if (proc_stop(obj) == -1)
    return (-1);
  /*
  ** Set/Unset options.
  */
  ctl[0] = PCUNSET;
  ctl[1] = PR_ASYNC;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
    return (-1);
  ctl[0] = PCUNSET;
  ctl[1] = PR_FORK;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
    return (-1);
  /*
  ** Define a set of hardware faults to be traced in the process.
  ** FLTTRACE = single step
  ** FLTBPT =  breakpoint
  */
  pcsfault.ctl = PCSFAULT;
  premptyset(&pcsfault.s_fltset);
  praddset(&pcsfault.s_fltset, FLTTRACE);
  praddset(&pcsfault.s_fltset, FLTBPT);
  if (write_procfs(obj, "ctl", sizeof(pcsfault_t), (char*) &pcsfault, 0) == -1)
    return (-1);
  /*
  ** Flush fds
  */
  close(((procfs_fd_t*) obj->luse)->ctl);
  ((procfs_fd_t*) obj->luse)->ctl = -1;
  close(((procfs_fd_t*) obj->luse)->status);
  ((procfs_fd_t*) obj->luse)->status = -1;
  close(((procfs_fd_t*) obj->luse)->ras);
  ((procfs_fd_t*) obj->luse)->ras = -1;
  close(((procfs_fd_t*) obj->luse)->was);
  ((procfs_fd_t*) obj->luse)->was = -1;
  close(((procfs_fd_t*) obj->luse)->gwin);
  ((procfs_fd_t*) obj->luse)->gwin = -1;
  /*
  ** Do one step to clear fault & sigs.
  */
  ctl[0] = PCRUN;
  ctl[1] = PRSABORT|PRCFAULT|PRCSIG|PRSTEP;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
    return (-1);
  /*
  ** Wait...
  */
  ctl[0] = PCWSTOP;
  ctl[1] = 0;
  while (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1 && errno == EINTR)
    (void) usleep(10);
  return (0);
}

int	proc_detach(proc_obj_t* obj)
{
  pcsfault_t	pcsfault;
  long		ctl[2];

  /*
  ** Clear tracing flags.
  */
  pcsfault.ctl = PCSFAULT;
  premptyset(&pcsfault.s_fltset);
  if (write_procfs(obj, "ctl", sizeof(pcsfault_t), (char*) &pcsfault, 0) == -1)
    return (-1);
  ctl[0] = PCSET;
  ctl[1] = PR_RLC;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
    return (-1);
  /*
  ** Continue.
  */
  ctl[0] = PCRUN;
  ctl[1] = PRSABORT|PRCFAULT|PRCSIG;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) != 2 * sizeof(long))
    return (-1);
  return (0);
}

int	proc_kill(proc_obj_t* obj)
{
  long	ctl[2];

  ctl[0] = PCKILL;
  ctl[1] = 0;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1)
    return (-1);
  return (0);
  return (-1);
}

int	proc_stop(proc_obj_t* obj)
{
  long	ctl[2];

  ctl[0] = PCDSTOP;
  ctl[1] = 0;
  if (write_procfs(obj, "ctl", 2 * sizeof(long), (char*) ctl, 0) == -1)
    return (-1);
  return (0);
}
