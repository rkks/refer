#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include "ssp.h"

static void sigpipe (int sig);

int main (void)
{
	int fd [2];
	int n;
	char buf [LINE_LEN];

	if (sigset (SIGPIPE, sigpipe) == SIG_ERR)
		err_msg ("sigset failed");

	if (pipe (fd) == -1)
		err_msg ("pipe failed");

	switch (fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			close (fd [0]);

			if (fd [1] != STDIN_FILENO) {
				if (dup2 (fd [1], STDIN_FILENO) != STDIN_FILENO)
					err_msg ("dup to standard input failed");
			}

			if (fd [1] != STDOUT_FILENO) {
				if (dup2 (fd [1], STDOUT_FILENO) != STDOUT_FILENO)
					err_msg ("dup to standard output failed");
			}

			execl ("./sqrt", "sqrt", NULL);
			err_msg ("exec failed");

		default:
			close (fd [1]);

			printf ("Enter a number: ");
			fflush (stdout);

			while (fgets (buf, LINE_LEN, stdin) != NULL) {
				n = strlen (buf);
				if (writen (fd [0], buf, n) == -1)
					err_msg ("writen failed");

				if ((n = read (fd [0], buf, LINE_LEN)) == -1)
					err_msg ("read from pipe failed");
				
				if (n == 0)
					err_msg ("Coprocess closed pipe");

				buf [n] = '\0';
				if (fputs (buf, stdout) == EOF)
					err_msg ("fputs failed");

				printf ("Enter a number: ");
				fflush (stdout);
			}

			if (ferror (stdin))
				err_msg ("fgets from standard input failed");

			return (0);
	}
}

static void sigpipe (int sig)
{
	printf ("Caught SIGPIPE\n");
	exit (1);
}
