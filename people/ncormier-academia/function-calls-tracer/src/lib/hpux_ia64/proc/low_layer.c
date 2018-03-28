/*
** I64_lowproc.c for ftrace in /u/ept3/barbot_u/work/ftrace/src/_julien
** 
** Made by julieN barbot
** Login   <barbot_u@epita.fr>
** 
** Started on  Sat Apr 22 17:22:46 2006 julieN barbot
** Last update Wed Apr 26 15:58:58 2006 julieN barbot
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/ttrace.h>
#include <sys/wait.h>
#include <machine/sys/uregs.h>

#include "arch_regs.h"
#include "api/includes/prototypes/proc/low_layer.h"
#define EXEC_EVENT (NOTHING_EVENT + 1)	// Event pour detecter l'appel au syscall exec()


proc_obj_t*	proc_new(void)
{
  proc_obj_t*	proc;

  proc = malloc(sizeof(*proc));
  proc->path = NULL;
  proc->pid = -1;
  proc->attach = 0;
  return proc;
}

int		proc_del(proc_obj_t* proc)
{
  free(proc);
  return 0;
}

int		proc_create(proc_obj_t* proc, char* bin_path, char** args)
{
  pid_t		child;
  int		pfd1[2], pfd2[2];
  char		c;
  ttevent_t	ev;

  ev.tte_events = TTEVT_SYSCALL|TTEVT_EXEC|TTEVT_EXIT|
    TTEVT_BPT_SSTEP|TTEVT_SIGNAL;
  ev.tte_opts = TTEO_NONE;

  /*
  ** Technique de Roumain avec des pipes
  ** pour synchoniser les deux process
  */
  if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
    return (-1);
  switch ((child = fork()))
    {
    case -1:
      return (-1);

      /*
      ** Child Process
      */
    case 0:
      if (setpgid(0, 0) == -1)
	return (-1);
      if (ttrace(TT_PROC_SETTRC, 0, 0, 0, TT_VERSION, 0) == -1)
	return -1;
      /* tell parent we are SETTRC'ed */
      if (write(pfd2[1], (void *)&c, sizeof c) != sizeof c)
	return (-1);
      /* wait for exec event to be set*/
      if (read(pfd1[0], (void *)&c, sizeof c) != sizeof c)
	return (-1);
      close(pfd1[0]); close(pfd1[1]); close(pfd2[0]); close(pfd2[1]);
      if (execvp(bin_path, args) == -1)
	return (-1);
      return (-1);
    }
  /*
  ** Ftrace process
  */
  if (read(pfd2[0], (void *)&c, sizeof c) != sizeof c)
    return (-1);
  proc->pid = child;
  if (ttrace(TT_PROC_SET_EVENT_MASK, proc->pid, 0,
	     (uint64_t)&ev, sizeof ev, 0) == -1)
    return (-1);
  /* tell the child to exec */
  if (write(pfd1[1], (void *)&c, sizeof c) != sizeof c)
    return (-1);
  close(pfd1[0]); close(pfd1[1]); close(pfd2[0]); close(pfd2[1]);

  /*
  ** TODO:
  ** Attendre la fin de l'exec()
  ** set des breakpoints
  */

  proc->luse = NULL;
  if (proc_wait_debug_event(proc, NULL) == -1)
    return (-1);

  return child;
}

int		proc_attach(proc_obj_t* proc, pid_t pid)
{
  ttevent_t	ev;
  siginfo_t	si;

  ev.tte_events = TTEVT_SYSCALL_ENTRY|TTEVT_SYSCALL_RESTART|TTEVT_SYSCALL_RETURN|
    TTEVT_EXEC|TTEVT_EXIT|TTEVT_BPT_SSTEP|TTEVT_SIGNAL;
  ev.tte_opts = TTEO_NONE;

  proc->pid = -1;
  if (ttrace(TT_PROC_ATTACH, pid, 0, TT_DETACH_ON_EXIT,
	       TT_VERSION, 0) == -1)
    return -1;
  proc->pid = pid;
  if (waitid(P_PID, pid, &si, WEXITED|WSTOPPED) < 0 ||
      si.si_pid != pid || si.si_code != CLD_STOPPED)
    return (-1);
  if (ttrace(TT_PROC_SET_EVENT_MASK, pid, 0,
	     (uint64_t)&ev, sizeof ev, 0) == -1)
    return -1;
  return (pid);
}

int		proc_detach(proc_obj_t* proc)
{
  return (ttrace(TT_PROC_DETACH, proc->pid, 0, 0, 0, 0));
}

static int	proc_wait(proc_obj_t* proc, int* sigret, ttwopt_t option)
{
  static
    ttstate_t	tts;
  int		rval;

  rval = ttrace_wait(proc->pid, 0, option, &tts, sizeof tts);
  if (rval == -1)
    return (-1);
  proc->luse = (void *)&tts;
  switch (tts.tts_event)
    {
    case TTEVT_SYSCALL_ENTRY:
    case TTEVT_SYSCALL_RESTART:
    case TTEVT_SYSCALL_RETURN:
    case TTEVT_BPT_SSTEP:
      return (DEBUG_EVENT);
    case TTEVT_EXEC:
      return (EXEC_EVENT);
    case TTEVT_EXIT:
      return (EXIT_EVENT);
    case TTEVT_SIGNAL:
      if (sigret != NULL)
	(*sigret) = tts.tts_u.tts_signal.tts_signo;
      return (SIGNAL_EVENT);
    case TTEVT_NONE:
    default:
      return (-1);
    }
  return (-1);
}

int	proc_wait_debug_event(proc_obj_t* proc, int* sigret)
{
  int	rval;

  switch (rval = proc_wait(proc, sigret, TTRACE_WAITOK))
    {
    case EXEC_EVENT:
      return (DEBUG_EVENT);
    default:
      return rval;
    }
  return (NOTHING_EVENT);
}

int		proc_wait_debug_event_nohang(proc_obj_t* proc, int* sigret)
{
  int	rval;

  switch (rval = proc_wait(proc, sigret, TTRACE_NOWAIT))
    {
    case EXEC_EVENT:
      return (DEBUG_EVENT);
    default:
      return rval;
    }
  return (NOTHING_EVENT);
}

int	proc_ttrace(proc_obj_t* proc, int signal,
		    ttreq_t request,  pid_t pid, lwpid_t lwpid,
		    uint64_t addr, uint64_t data, uint64_t addr2)
{
  ttstate_t	tts;

  if (proc->luse == NULL)
    return (-1);
  tts = *((ttstate_t *)proc->luse);

  if (signal == 0 && tts.tts_u.tts_signal.tts_signo != 0)
    {
      printf("Signal: %d -- %d\n", signal, tts.tts_u.tts_signal.tts_signo);
      signal = tts.tts_u.tts_signal.tts_signo;
      if (signal == 9)
	signal = 0;
      data = signal;
    }
  return (ttrace(request, pid, lwpid,
		 addr, data, addr2));
}

int		proc_step(proc_obj_t* proc, int signal)
{
  ttstate_t	tts;

  if (proc->luse == NULL)
    return (-1);
  tts = *((ttstate_t *)proc->luse);

  return (proc_ttrace(proc, signal,
		      TT_LWP_SINGLE, proc->pid, tts.tts_lwpid,
		      TT_NOPC, signal, 0L));
}

int		proc_step_addr(proc_obj_t* proc, addr_t addr, int signal)
{
  ttstate_t	tts;

  if (proc->luse == NULL)
    return (-1);
  tts = *((ttstate_t *)proc->luse);

  return (proc_ttrace(proc, signal,
		      TT_LWP_SINGLE, proc->pid, tts.tts_lwpid,
		      addr, signal, 0L));
}

int		proc_continue(proc_obj_t* proc, int signal)
{
  return (proc_ttrace(proc, signal,
		      TT_PROC_CONTINUE, proc->pid, 0,
		      TT_NOPC, signal, 0L));
}

int		proc_continue_addr(proc_obj_t* proc, addr_t addr, int signal)
{
  ttstate_t	tts;

  if (proc->luse == NULL)
    return (-1);
  tts = *((ttstate_t *)proc->luse);

  return (proc_ttrace(proc, signal,
		      TT_LWP_CONTINUE, proc->pid, tts.tts_lwpid,
		      addr, signal, 0L));
}

int		proc_stop(proc_obj_t* proc)
{
  return (ttrace(TT_PROC_STOP, proc->pid, 0, 0, 0, 0));
}


int		proc_kill(proc_obj_t* proc)
{
  return (ttrace(TT_PROC_EXIT, proc->pid, 0, 0, 0, 0));
}

regs_t		*proc_get_registers(proc_obj_t* proc)
{
  regs_t*	regs;
  ttstate_t*	tts;

  if (proc->luse == NULL)
    return (-1);
  tts = (ttstate_t *)proc->luse;

  regs = malloc(sizeof(regs_t));
  if (ttrace(TT_LWP_RUREGS, proc->pid, tts->tts_lwpid,
	     (uint64_t)__reason, (uint64_t)8, (uint64_t)&regs->reason) == -1 ||
      ttrace(TT_LWP_RUREGS, proc->pid, tts->tts_lwpid,
	     (uint64_t)__ip, (uint64_t)8, (uint64_t)&regs->ip) == -1 ||
      ttrace(TT_LWP_RUREGS, proc->pid, tts->tts_lwpid,
	     (uint64_t)__r12, (uint64_t)8, (uint64_t)&regs->sp) == -1 ||
      ttrace(TT_LWP_RUREGS, proc->pid, tts->tts_lwpid,
	     (uint64_t)__ar_bsp, (uint64_t)8, (uint64_t)&regs->bsp) == -1 ||
      ttrace(TT_LWP_RUREGS, proc->pid, tts->tts_lwpid,
	     (uint64_t)__ar_pfs, (uint64_t)8, (uint64_t)&regs->pfs) == -1)
    {
      free(regs);
      return (NULL);
    }
  return (regs);
}

int	proc_set_registers(proc_obj_t* proc, regs_t* regs)
{
  proc = NULL;
  regs = NULL;
  return (-1);
}

int	proc_read(proc_obj_t* proc, addr_t addr, unsigned int i, char* c)
{
  proc = NULL; addr = NULL;
  i = 0; c = NULL;
  return (-1);
}

int	proc_write(proc_obj_t* proc, addr_t addr, unsigned int i, char* c)
{
  proc = NULL; addr = NULL;
  i = 0; c = NULL;
  return (-1);
}
