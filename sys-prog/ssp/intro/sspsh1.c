#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "ssp.h"

int main (void)
{
	char buf [LINE_LEN];
	pid_t pid;
	int status;

	printf ("-> ");
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

	return (0);
}
