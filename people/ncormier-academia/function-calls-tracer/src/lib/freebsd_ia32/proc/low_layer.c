/*
** low_layer.c for  in /home/nico/lang/c/ftrace/src/api/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:03:53 2006 nicolas
** Last update Thu Apr 27 00:01:39 2006 nicolas
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>

#include "api/includes/prototypes/proc/low_layer.h"

#include "includes/error.h"

proc_obj_t*	proc_new(void)
{
  proc_obj_t*	new = NULL;

  new  = malloc(sizeof(proc_obj_t));
  if (new == NULL)
    return (NULL);

  new->path = NULL;
  new->pid = -1;
  new->attach = -1;
  new->luse = NULL;
  return (new);
}

int	proc_del(proc_obj_t* obj)
{
  free(obj);
  return (0);
}

regs_t*	proc_get_registers(proc_obj_t* obj)
{
  regs_t*	ret;

  ret = malloc(sizeof(regs_t));
  if (ret == NULL)
    return (NULL);

  errno = 0;
  ptrace(PT_GETREGS, obj->pid, (caddr_t) ret, 0);
  if (errno)
    return (NULL);

  return (ret);
}

int	proc_set_registers(proc_obj_t* obj, regs_t* registers)
{
  errno = 0;
  ptrace(PT_SETREGS, obj->pid, (caddr_t) registers, 0);
  if (errno)
    return (-1);
  return (0);
}

int	proc_read(proc_obj_t* obj, addr_t raddr, unsigned int len, char* laddr)
{
  long	addr, val;
  int	ret = 0;
  int	m;

  addr = (long) raddr;

/*   if (addr & (sizeof(long) - 1)) */
/*     { */
/*       errno = EFTRACE; */
/*       ftrace_errstr = "Bad Align"; */
/*       return (-1); */
/*     } */
  while (len)
    {
      errno = 0;
      val = ptrace(PT_READ_D, obj->pid, (char *) addr, 0);
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
** Must be align with a long!
*/
int	proc_write(proc_obj_t* obj, addr_t raddr, unsigned int len, char* laddr)
{
  long	addr, val;
  int	ret = 0;
  int	m;

  addr = (long) raddr;

/*   if (addr & (sizeof(long) - 1)) */
/*     { */
/*       errno = EFTRACE; */
/*       ftrace_errstr = "Bad Align"; */
/*       return (-1); */
/*     } */
  while (len)
    {
      errno = 0;
      val = ptrace(PT_WRITE_D, obj->pid, (char *) addr, *((long*) laddr));
      if (errno)
	return (-1);
      m = MIN((int) sizeof(long), len);
      addr += sizeof(long);
      laddr += m;
      len -= m;
      ret += m;
    }
  return (ret);
}

int	proc_continue(proc_obj_t* obj, int signal)
{
  errno = 0;
  ptrace(PT_CONTINUE, obj->pid, (caddr_t) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_continue_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  regs_t	regs;

  errno = 0;
  ptrace(PT_GETREGS, obj->pid, (caddr_t) &regs, 0);
  if (errno)
    return (-1);

  regs.r_eip = addr;

  errno = 0;
  ptrace(PT_SETREGS, obj->pid, (caddr_t) &regs, 0);
  if (errno)
    return (-1);

  errno = 0;
  ptrace(PT_CONTINUE, obj->pid, (caddr_t) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_step(proc_obj_t* obj, int signal)
{
  errno = 0;
  ptrace(PT_STEP, obj->pid, (caddr_t) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_step_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  regs_t	regs;

  errno = 0;
  ptrace(PT_GETREGS, obj->pid, (caddr_t) &regs, 0);
  if (errno)
    return (-1);

  regs.r_eip = addr;

  errno = 0;
  ptrace(PT_SETREGS, obj->pid, (caddr_t) &regs, 0);
  if (errno)
    return (-1);

  errno = 0;
  ptrace(PT_STEP, obj->pid, (caddr_t) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_wait_debug_event(proc_obj_t* obj, int* sigret)
{
  int	ret, status, signal;


  *sigret = -1;

  ret = wait4(obj->pid, &status, 0, NULL);
  if (ret == -1)
    return (-1);

  if(WIFSTOPPED(status))
    {
      signal = WSTOPSIG(status);
      switch(signal)
	{
	case SIGTRAP:
	  return (DEBUG_EVENT);
	default:
	  *sigret = signal;
	  return (SIGNAL_EVENT);
	}
    }
  else if (WIFEXITED(status))
    {
      return (EXIT_EVENT);
    }
  else if (WIFSIGNALED(status))
    {
      signal = WTERMSIG(status);
      *sigret = signal;
      return (EXIT_SIGNALED_EVENT);
    }
  errno = EFTRACE;
  ftrace_errstr = "Unknown Event";
  return (-1);
}

int	proc_wait_debug_event_nohang(proc_obj_t* obj, int* sigret)
{
  int	ret, status, signal;


  *sigret = -1;

  ret = wait4(obj->pid, &status, WNOHANG, NULL);
  if (ret == -1)
    return (-1);

  if (ret == 0)
    return (NOTHING_EVENT);

  if(WIFSTOPPED(status))
    {
      signal = WSTOPSIG(status);
      switch(signal)
	{
	case SIGTRAP:
	  return (DEBUG_EVENT);
	default:
	  *sigret = signal;
	  return (SIGNAL_EVENT);
	}
    }
  else if (WIFEXITED(status))
    {
      return (EXIT_EVENT);
    }
  else if (WIFSIGNALED(status))
    {
      signal = WTERMSIG(status);
      *sigret = signal;
      return (EXIT_SIGNALED_EVENT);
    }
  errno = EFTRACE;
  ftrace_errstr = "Unknown Event";
  return (-1);
}

/*
** Tracer has just attached/created process.
** Wait until process is ready.
*/
static int	wait_for_tracee(int pid)
{
  int	ret, status, signal;

  while (1)
    {
      ret = wait4(pid, &status, 0, NULL);
      if (ret == -1)
	break;
      if(WIFSTOPPED(status))
	{
	  signal = WSTOPSIG(status);
	  switch(signal)
	    {
	    case SIGTRAP:
	    case SIGSTOP:
	      /*
	      ** Ready !
	      */
	      return (0);
	    default:
	      errno = 0;
	      ptrace(PT_CONTINUE, pid, (caddr_t) 1, signal);
	      if (errno)
		return (-1);
	      break;
	    }
	}
      else
	{
	  errno = EFTRACE;
	  ftrace_errstr = "Abnormal Process Exit";
	  break;
	}
    }
  return (-1);
}


/*
** Create Process And Suspend it.
*/
int	proc_create(proc_obj_t* obj, char* path, char** argv)
{
  int	son;

  if (access(path, X_OK) == -1)
    return (-1);
  if ((son = fork()) == -1)
    return (-1);
  /*
  ** Father exec,
  ** Waiting for tracee.
  */
  if (son)
    {
      /*
      ** Wait son ready for tracing.
      */
      if (wait_for_tracee(son) == -1)
	return (-1);

      obj->pid = son;
      obj->path = path;
      obj->attach = 0;
      return (1);
    }
  /*
  ** Son exec
  */
  if (setpgid(0, 0) == -1)
    exit(1);
  errno = 0;
  ptrace(PT_TRACE_ME, 0, (char *) 1, 0);
  if (errno)
    exit(1);
  execv(path, argv);
  exit(1);
  return (-1);
}

/*
** Attach Process And Suspend it.
*/
int	proc_attach(proc_obj_t* obj, pid_t pid)
{
  int	son;

  son = pid;
  errno = 0;
  ptrace(PT_ATTACH, son, NULL, 0);
  if (errno)
    return (-1);
  obj->pid = son;
  obj->path = NULL;
  obj->attach = 1;
  if (setpgid(0, 0) == -1)
    return (-1);
  /*
  ** Wait son ready for tracing.
  */
  if (wait_for_tracee(son) == -1)
    return (-1);
  return (1);
}

int	proc_detach(proc_obj_t* obj)
{
  errno = 0;
  ptrace(PT_DETACH, obj->pid, (char *) 1, 0);
  if (errno)
    return (-1);
  return (0);
}

int	proc_kill(proc_obj_t* obj)
{
  /*
  ** like PTRACE CONTINUE with SIGKILL
  */
  errno = 0;
  ptrace(PT_KILL, obj->pid, NULL, 0);
  if (errno)
    return (-1);
  return (0);
}

int	proc_stop(proc_obj_t* obj)
{
  if (kill(obj->pid, SIGTRAP) == -1)
    return (-1);
  return (0);
}


/* static inline void SETBITS(unsigned int* dw, int low_bit, int bits, int new_value) */
/* { */
/*   int mask; */

/*   mask = (1 << bits) - 1;  */
/*   *dw = (*dw & ~(mask << low_bit)) | (new_value << low_bit); */
/* } */

/* int	proc_set_hardware_breakpoint(proc_obj_t* obj, brkp_t* brkp, int type) */
/* { */
/*   struct dbreg	dbregs; */
/*   unsigned int	dr7; */
/*   unsigned int	i; */

/*   errno = 0; */
/*   ptrace(PT_GETDBREGS, obj->pid, (caddr_t) &dbregs, 0); */
/*   if (errno) */
/*     return (-1); */

/*   /\* */
/*   ** For some mysterious reason, some of the reserved bits in the */
/*   ** debug control register get set.  Mask these off, otherwise the */
/*   ** ptrace call below will fail. */
/*   *\/ */
/*   DBREG_DRX ((&dbregs), 7) &= ~(0x0000fc00); */


/*   for(i = 0; i < 4; i++) */
/*     { */
/*       if((dr7 & (1 << (i * 2))) == 0) */
/* 	{ */
/* 	  /\* */
/* 	  ** Set DR{0,1,2,3} addr */
/* 	  *\/ */
/* 	  DBREG_DRX((&dbregs), i) = brkp->addr; */
/* 	  /\* */
/* 	  ** Set DR7 */
/* 	  *\/ */
/* 	  dr7 = DBREG_DRX((&dbregs), 7); */
/* 	  switch (type) */
/* 	    { */
/* 	    case DB_EXEC: */
/* 	      SETBITS(&dr7, 16 + i * 4, 2, DBREG_DR7_EXEC); */
/* 	      SETBITS(&dr7, 18 + i * 4, 2, 0); */
/* 	      break; */
/* 	    case DB_WRONLY: */
/* 	      SETBITS(&dr7, 16 + i * 4, 2, DBREG_DR7_WRONLY); */
/* 	      SETBITS(&dr7, 18 + i * 4, 2, 3); */
/* 	      break; */
/* 	    case DB_RDWR: */
/* 	      SETBITS(&dr7, 16 + i * 4, 2, DBREG_DR7_RDWR); */
/* 	      SETBITS(&dr7, 18 + i * 4, 2, 3); */
/* 	      break; */
/* 	    } */
/* 	  DBREG_DRX((&dbregs), 7) = dr7; */
/* 	  /\* */
/* 	  ** Set Breakpoint */
/* 	  *\/ */
/* 	  errno = 0; */
/* 	  ptrace(PT_SETDBREGS, obj->pid, (caddr_t) &dbregs, 0); */
/* 	  if (errno) */
/* 	    return (-1); */
/* 	  brkp->set = 1; */
/* 	  return (0); */
/* 	} */
/*     } */
/*   return (-1); */
/* } */
