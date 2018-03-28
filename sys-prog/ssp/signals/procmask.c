#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "ssp.h"

int print_proc_mask (const char *msg);

int main (void)
{
	sigset_t set;

	print_proc_mask ("Before");

	sigemptyset (&set);
	sigaddset (&set, SIGABRT);
	sigaddset (&set, SIGALRM);
	if (sigprocmask (SIG_SETMASK, &set, NULL) == -1)
		err_msg ("sigprocmask failed");

	print_proc_mask ("After");

	return (0);
}

int print_proc_mask (const char *msg)
{
	sigset_t set;
	int old_errno;

	old_errno = errno;
	if (sigprocmask (0, NULL, &set) == -1)
		return (-1);

	if (msg)
		printf ("%s: ", msg);

	if (sigismember (&set, SIGABRT))
		printf ("SIGABRT ");
	if (sigismember (&set, SIGALRM))
		printf ("SIGALRM ");
	if (sigismember (&set, SIGBUS))
		printf ("SIGBUS ");
	if (sigismember (&set, SIGCANCEL))
		printf ("SIGCANCEL ");

	printf ("\n");

	errno = old_errno;

	return (0);
}
