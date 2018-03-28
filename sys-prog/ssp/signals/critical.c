#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "ssp.h"

static void sigint (int sig);
extern int print_proc_mask (const char *msg);

int main (void)
{
	sigset_t old_mask;
	sigset_t new_mask;
	sigset_t empty_mask;

	if (sigset (SIGINT, sigint) == SIG_ERR)
		err_msg ("sigset failed");

	print_proc_mask ("Before");

	sigemptyset (&empty_mask);

	sigemptyset (&new_mask);
	sigaddset (&new_mask, SIGINT);
	if (sigprocmask (SIG_BLOCK, &new_mask, &old_mask) == -1)
		err_msg ("sigprocmask failed");

	print_proc_mask ("In critical code region");

	if (sigsuspend (&empty_mask) == -1)
		if (errno != EINTR)
			err_msg ("sigsuspend failed");
	print_proc_mask ("After sigsuspend returns");

	if (sigprocmask (SIG_SETMASK, &old_mask, NULL) == -1)
		err_msg ("sigpromask failed");
	print_proc_mask ("After");

	return (0);
}

static void sigint (int sig)
{
	print_proc_mask ("\nIn SIGINT signal handler");
}
