/*
** netmalloc.c for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/fcntl.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/signalvar.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <sys/cdefs.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/sysent.h>
#include <sys/module.h>
#include <sys/kthread.h>
#include <sys/resource.h>
#include <sys/resourcevar.h>
#include <machine/memdev.h>

#include "server.h"
#include "die.h"
#include "netmalloc_common.h"

/* File trigger deamon
 */
static void netmallocd_ep(void)
{
#if 0
  /* Kldstat for coredump
   */
  int i;
  void* waddr = &i;
  (void) tsleep(waddr, PCATCH|PWAIT, "idle", 1000);
  printf("LAUNCH!\n");
#endif
  server_t* serv = get_server();
  if (!serv) die("netmallocd_ep: get_server failed");
  serv->init_with_port(serv, 4242);
  serv->loop(serv);
}

static struct proc*		netmallocd_proc;

static struct kproc_desc	netmallocd_kp = {
  "netmallocd",
  netmallocd_ep,
  &netmallocd_proc
};

SYSINIT(netmallocd, SI_SUB_KTHREAD_BUF, SI_ORDER_FIRST, kproc_start, &netmallocd_kp)
