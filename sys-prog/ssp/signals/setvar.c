#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "ssp.h"

static volatile sig_atomic_t flag;

static void sig_handler (int sig);

int main (void)
{
	sigset_t old_mask;
	sigset_t new_mask;
	sigset_t empty_mask;

	if (sigset (SIGINT, sig_handler) == SIG_ERR)
		err_msg ("sigset (SIGINT) failed");
	if (sigset (SIGQUIT, sig_handler) == SIG_ERR)
		err_msg ("sigset (SIGQUIT) failed");

	sigemptyset (&empty_mask);

	sigemptyset (&new_mask);
	sigaddset (&new_mask, SIGQUIT);
	if (sigprocmask (SIG_BLOCK, &new_mask, &old_mask) == -1)
		err_msg ("sigprocmask failed");

	while (flag == 0) {
		if (sigsuspend (&empty_mask) == -1)
			if (errno != EINTR)
				err_msg ("sigsuspend failed");
	}

	flag = 0;

	if (sigprocmask (SIG_SETMASK, &old_mask, NULL) == -1)
		err_msg ("sigpromask failed");

	return (0);
}

static void sig_handler (int sig)
{
	if (sig == SIGINT)
		printf ("\nReceived SIGINT\n");
	else if (sig == SIGQUIT)
		flag = 1;
}
