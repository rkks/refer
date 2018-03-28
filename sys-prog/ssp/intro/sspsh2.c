#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "ssp.h"

static void sigint (int sig);

int main (void)
{
	char buf [LINE_LEN];
	pid_t pid;
	int status;

	if (sigset (SIGINT, sigint) == SIG_ERR)
		err_msg ("sigset");

	printf ("-> ");

	for (;;) {
		while (fgets (buf, LINE_LEN, stdin) != NULL) {
			if (*buf != '\0')
				buf [strlen (buf) - 1] = '\0';
			if (strlen (buf) == 0) {
				printf ("-> ");
				continue;
			}

			switch (pid = fork ()) {
				case -1:
					err_msg ("Can't fork");
					break;

				case 0:
					/*
					 * Child process
					 */
					execlp (buf, buf, NULL);
					err_msg ("Can't exec %s", buf);
					break;

				default:
					/*
					 * Parent process
					 */
					if (waitpid (pid, &status, 0) == -1)
						err_ret ("waitpid");
					break;
			}

			printf ("-> ");
		}

		if (errno != EINTR)
			break;

		errno = 0;
	}

	return (0);
}

static void sigint (int sig)
{
	printf ("Caught SIGINT\n");
	printf ("-> ");
}
