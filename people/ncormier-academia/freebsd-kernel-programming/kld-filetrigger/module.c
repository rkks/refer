/*
** module.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

/* Headers
 */
#include <sys/param.h>
#include <sys/cdefs.h>
#include <sys/kernel.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/sysent.h>
#include <sys/module.h>
#include <sys/kthread.h>
#include <sys/proc.h>
#include "ftrigger.h"
#include "die.h"
#include "server.h"

/* File trigger deamon
 */
static void ftriggerd_ep(void)
{
#if 0
  /* Kldstat for coredump
   */
  int i;
  void* waddr = &i;
  (void) tsleep(waddr, PCATCH|PWAIT, "idle", 1000);
  printf("LAUNCH!\n");
#endif
  launch_ftrigger_server();
}

static struct proc *ftriggerd_proc;

static struct kproc_desc ftriggerd_kp = {
  "ftriggerd",
  ftriggerd_ep,
  &ftriggerd_proc
};

SYSINIT(ftriggerd, SI_SUB_KTHREAD_BUF, SI_ORDER_FIRST, kproc_start, &ftriggerd_kp)

/* File trigger syscall module
 * QAD
 */

struct syscall_args
{
  int		type;
  char*		str_a;
  short		s_a;
  unsigned	ui_a;
  char*		str_b;
};

struct ftrigger_sysc_args
{
  struct syscall_args*	args;
};

static int	ftrigger_sysc(struct proc *p, struct ftrigger_sysc_args *user_args)
{
  char kstr_a[1024] = "";
  char kstr_b[1024] = "";
  int size;
  int err = 0;
  struct syscall_args* args = user_args->args;

  if (args->str_a)
    {
      err = copyinstr(args->str_a, &kstr_a, sizeof(kstr_a), &size);
      if (err == EFAULT)
	return err;
    }
  if (args->str_b)
    {
      err = copyinstr(args->str_b, &kstr_b, sizeof(kstr_b), &size);
      if (err == EFAULT)
	return err;
    }
  if (ftrigger_syscall_interface(args->type, kstr_a, args->s_a, args->ui_a, kstr_b))
    return EFAULT;
  return 0;
}

static struct sysent ftrigger_sysc_sysent = 
{
  2, /* Number of parameters */ 
  (sy_call_t *)ftrigger_sysc	/* syscall func */ 
};

static int syscall_num = NO_SYSCALL;

static int	module_handler(struct module *m, int what, void *arg)
{
  int err = 0;
  switch (what) {
  case MOD_LOAD:
    uprintf("Client system call loaded at slot: %d\n", syscall_num);
    break;
  case MOD_UNLOAD:
    break;
  default:
    err = EINVAL;
    break;
  }
  return(err);
}

SYSCALL_MODULE(ftrigger_sysc,  /* name */
	       &syscall_num, /* syscall number */
	       &ftrigger_sysc_sysent, /* sysent struct */
               module_handler, /* module handler func */
	       NULL); /* module handler arg */
