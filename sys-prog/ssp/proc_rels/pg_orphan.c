#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "ssp.h"

static void print_ids (char *proc);

int main (void)
{
	char c;

	print_ids ("Parent");

	switch (fork ()) {
		case -1:	
			err_msg ("fork failed");
			break;

		case 0:
			print_ids ("Child");
			sigset (SIGHUP, SIG_IGN);
			kill (getpid (), SIGTSTP);

			printf ("Received SIGCONT; continuing...\n");
			print_ids ("Child");

			if (read (STDIN_FILENO, &c, 1) != 1)
				err_msg ("Read error from controlling terminal");

			_exit (0);

		default:
			sleep (2);
			exit (0);
	}

	return (0);
}

static void print_ids (char *proc)
{
	printf ("%s: PID = %ld, PPID = %ld, PGID = %ld\n",
		proc, (long) getpid (), (long) getppid (), (long) getpgrp ());
	fflush (stdout);
}
