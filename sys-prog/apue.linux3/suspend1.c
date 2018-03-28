#include	<signal.h>
#include	"ourhdr.h"

static void	sig_int(int);

int
main(void)
{
	sigset_t	newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");

	sigemptyset(&zeromask);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
					/* block SIGINT and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

					/* critical region of code */
	pr_mask("in critical region: ");

					/* allow all signals and pause */
	if (sigsuspend(&zeromask) != -1)
		err_sys("sigsuspend error");
	pr_mask("after return from sigsuspend: ");

					/* reset signal mask which unblocks SIGINT */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");

					/* and continue processing ... */
	exit(0);
}

static void
sig_int(int signo)
{
	pr_mask("\nin sig_int: ");
	return;
}
