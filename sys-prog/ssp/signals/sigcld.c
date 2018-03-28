#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include "ssp.h"

static void sigcld (int sig);

int main (void)
{
	pid_t pid;

	if (signal (SIGCLD, sigcld) == SIG_ERR)
		err_msg ("signal failed");

	if ((pid = fork ()) == -1)
		err_msg ("fork failed");

	if (pid == 0) {
		sleep (1);
		_exit (0);
	}

	pause ();
	return (0);
}

static void sigcld (int sig)
{
	pid_t pid;
	int status;

	printf ("Received SIGCLD\n");

	if (signal (SIGCLD, sigcld) == SIG_ERR)
		err_msg ("signal failed");

	if ((pid = wait (&status)) == -1)
		err_msg ("wait failed");

	printf ("Child PID = %ld\n", (long) pid);
}
