#include	<signal.h>
#include	<stddef.h>
#include	"ourhdr.h"

static sigset_t	newmask, oldmask;

void
sig_block(int signo)
{
	sigemptyset(&newmask);
	sigaddset(&newmask, signo);
			/* block specified signal and save current mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");
}

void
sig_unblock(void)
{
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		log_sys("SIG_SETMASK error");
}
