/*
	Synchronize processes with signals
	AUP2, Sec. 6.02, 9.02.3 (not in book)

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include "pcsync_sig.h"

#define NO_OPx

/*
	In some systems, a signal sent to a child between the time that fork() returns to the parent and the time that the child starts executing may be lost. (It is pending when it is sent, and then pending signals are cleared.) This bug is so common that it is discussed in the POSIX rationale for fork().

	I can't think of a fix for this using only signals. But, see pcsync.c for a correct and efficient implementation using pipes, with no signals at all.

	(Note: One might think that the answer is for the child to signal the parent when is is fully baked, with the parent waiting for that signal before signalling the child. But this method works only for a single child. It doesn't scale up for multiple children created all at once.)
*/

static void signal_handler_child(int signo)
{
}

bool pcsync_init(struct pcsync *p)
{
#ifndef NO_OP
	sigset_t set;

	(void)sigemptyset(&set);
	(void)sigaddset(&set, SIGUSR1);
	ec_neg1( sigprocmask(SIG_BLOCK, &set, &p->old_set) )
#endif /* NO_OP */
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_wait_for_parent(struct pcsync *p)
{
#ifndef NO_OP
	struct sigaction act;
	int r;
	sigset_t suspendset;

	memset(&act, 0, sizeof(act));
	act.sa_handler = signal_handler_child;
	ec_neg1( sigaction(SIGUSR1, &act, NULL) )
	sigfillset(&suspendset);
	sigdelset(&suspendset, SIGUSR1);
	r = sigsuspend(&suspendset);
	if (r == -1 && errno != EINTR)
		EC_FAIL
	ec_neg1( sigprocmask(SIG_SETMASK, &p->old_set, NULL) )
#endif /* NO_OP */
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_unblock_children(struct pcsync *p, int children,
  pid_t *pid_child)
{
#ifndef NO_OP
	int i;

	ec_neg1( sigprocmask(SIG_SETMASK, &p->old_set, NULL) )
	for (i = 0; i < children; i++)
		ec_neg1( kill(pid_child[i], SIGUSR1) )
#endif /* NO_OP */
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_end(struct pcsync *p)
{
	return true;
}

