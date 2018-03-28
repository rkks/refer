/*
** low_layer.c for  in /home/nico/lang/c/ftrace/src/api/core
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Thu Mar 23 13:03:53 2006 nicolas
** Last update Wed Apr 26 15:47:00 2006 nicolas rimbeau
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/fault.h>
#include <sys/syscall.h>
#include <sys/procfs.h>
#include <alpha/pcb.h>
#include <fcntl.h>

long	ptrace(long int request, long int process, ...);
int	sprintf(char *string,const char *format, ...);

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
  return (new);
}

int	proc_del(proc_obj_t* obj)
{
  free(obj);
  return (0);
}

#if 0
/*
** Procfs.
*/
static int	procfs_fd = -1;
regs_t*		proc_get_registers(proc_obj_t* obj)
{
  regs_t*	ret;
  prstatus_t	prs;


  if (procfs_fd == -1)
    {
      char	buf[256];

      sprintf(buf, "/proc/%d", obj->pid);
      if ((procfs_fd = open(buf, O_RDWR, 0)) == -1)
	return (NULL);
    }

  memset(&prs, 0, sizeof(prs));
  if (ioctl(procfs_fd, PIOCSTATUS, &prs) == -1)
    return (NULL);

  ret = malloc(sizeof(regs_t));
  if (ret == NULL)
    return (NULL);

  memcpy((char*) &(ret->greg), prs.pr_reg.regs, sizeof(gregset_t));

  return (ret);
}
#else
regs_t*		proc_get_registers(proc_obj_t* obj)
{
  regs_t*	ret;

  ret = malloc(sizeof(regs_t));
  if (ret == NULL)
    return (NULL);

  errno = 0;
  ret->r_pc = ptrace(PT_READ_U, obj->pid, PC, 0);
  ret->r_sp = ptrace(PT_READ_U, obj->pid, REG_BASE + 30, 0);
  ret->r_ra = ptrace(PT_READ_U, obj->pid, REG_BASE + 26, 0);
  ret->r_rv = ptrace(PT_READ_U, obj->pid, REG_BASE, 0);
  ret->r_fp = ptrace(PT_READ_U, obj->pid, REG_BASE + 15, 0);
  if (errno)
    {
      free(ret);
      return (NULL);
    }
  return (ret);
}
#endif

int	proc_set_registers(proc_obj_t* obj, regs_t* registers)
{
  obj = NULL;
  registers = NULL;
  return (0);
}

int	proc_read(proc_obj_t* obj, addr_t raddr, unsigned int len, char* laddr)
{
  long	addr, val;
  int	ret = 0;
  int	m;

  addr = (long) raddr;

  while (len)
    {
      errno = 0;
      val = ptrace(PT_READ_D, obj->pid, addr, 0);
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

  while (len)
    {
      errno = 0;
      val = ptrace(PT_WRITE_D, obj->pid, (ulong*) addr, *((long*) laddr));
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
  ptrace(PT_CONTINUE, obj->pid, (ulong*) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_continue_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  obj = NULL;
  addr = 0;
  signal = 0;
  return (-1);
}

int	proc_step(proc_obj_t* obj, int signal)
{
  errno = 0;
  ptrace(PT_STEP, obj->pid, (ulong*) 1, signal);
  if (errno)
    return (-1);
  return (0);
}

int	proc_step_addr(proc_obj_t* obj, addr_t addr, int signal)
{
  obj = NULL;
  addr = 0;
  signal = 0;
  return (-1);
}

int	proc_wait_debug_event(proc_obj_t* obj, int* sigret)
{
  int	ret, status, signal;

  ret = wait4(obj->pid, (union wait *) &status, 0, NULL);
  if (ret == -1)
    return (-1);

  *sigret = -1;

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
      ret = wait4(pid, (union wait *) &status, 0, NULL);
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
	      ptrace(PT_CONTINUE, pid, (ulong*) 1, signal);
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
  ptrace(PT_TRACE_ME, 0, (ulong*) 1, 0);
  if (errno)
    exit(1);
  execv(path, argv);
  exit(1);
  return (-1);
}

int	proc_attach(proc_obj_t* obj, pid_t pid)
{
  obj = NULL;
  pid = 0;
  return (-1);
}

int	proc_detach(proc_obj_t* obj)
{
  obj = NULL;
  return (-1);
}

int	proc_kill(proc_obj_t* obj)
{
  obj = NULL;
  return (-1);
}

int	proc_stop(proc_obj_t* obj)
{
  if (kill(obj->pid, SIGTRAP) == -1)
    return (-1);
  return (0);
}
