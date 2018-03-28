#include <stdio.h>
#include <signal.h>
#include "ssp.h"

extern int ssp_system (const char *string);
static void sigint (int sig);
static void sigchld (int sig);

int main (void)
{
	if (sigset (SIGINT, sigint) == SIG_ERR)
		err_msg ("sigset (SIGINT) failed");
	if (sigset (SIGCHLD, sigchld) == SIG_ERR)
		err_msg ("sigset (SIGCHLD) failed");

	if (ssp_system ("/bin/ex") == -1)
		err_msg ("ssp_system failed");

	return (0);
}

static void sigint (int sig)
{
	printf ("Caught SIGINT\n");
}

static void sigchld (int sig)
{
	printf ("Caught SIGCHLD\n");
}
