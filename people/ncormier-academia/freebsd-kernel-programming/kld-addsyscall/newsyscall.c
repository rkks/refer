/*
** newsyscall.c for  in /home/nico/lang/kernel/kld-addsyscall
** 
** Made by nicolas
*/

#include <sys/param.h>
#include <sys/cdefs.h>
#include <sys/kernel.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/sysent.h>
#include <sys/module.h>


struct my_syscall_args
{
  	char *str;
	int val;
};

static int	my_syscall(struct proc *p, struct my_syscall_args *user_args)
{
  char kstr[1024];
  int err = 0;
  int size = 0;

  err = copyinstr(user_args->str, &kstr, user_args->val, &size);
  if (err == EFAULT) 
    return err;
  uprintf("str = %s\n", kstr); 
  return 0;
}

static struct sysent my_syscall_sysent = 
{
  2,				/* Number of parameters */ 
  (sy_call_t *)my_syscall	/* syscall func */ 
};

static int syscall_num = NO_SYSCALL;

static int	module_handler(struct module *m, int what, void *arg)
{
  int err = 0;
    
  switch (what) {
  case MOD_LOAD:
    uprintf("System call loaded at slot: %d\n", syscall_num);
    break;
  case MOD_UNLOAD:
    uprintf("System call unloaded from slot: %d\n", syscall_num);
    break;
  default:
    err = EINVAL;
    break;
  }
  return(err);
}

SYSCALL_MODULE(my_syscall,  /* name */
	       &syscall_num, /* syscall number */
	       &my_syscall_sysent, /* sysent struct */
               module_handler, /* module handler func */
	       NULL); /* module handler arg */

