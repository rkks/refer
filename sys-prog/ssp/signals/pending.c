#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "ssp.h"

static void sigint (int sig);

int main (void)
{
	sigset_t old_mask;
	sigset_t new_mask;
	sigset_t pending_mask;

	if (sigset (SIGINT, sigint) == SIG_ERR)
		err_msg ("sigset failed");

	sigemptyset (&new_mask);
	sigaddset (&new_mask, SIGINT);
	if (sigprocmask (SIG_BLOCK, &new_mask, &old_mask) == -1)
		err_msg ("sigprocmask failed");
	printf ("SIGINT blocked\n");

	sleep (5);

	if (sigpending (&pending_mask) == -1)
		err_msg ("sigpending failed");
	if (sigismember (&pending_mask, SIGINT))
		printf ("\nSIGINT is pending\n");

	if (sigprocmask (SIG_SETMASK, &old_mask, NULL) == -1)
		err_msg ("sigpromask failed");
	printf ("SIGINT unblocked\n");

	sleep (5);
	perror ("After 2nd sleep");
	return (0);
}

static void sigint (int sig)
{
	printf ("Caught SIGINT\n");
}
