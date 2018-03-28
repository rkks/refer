#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssp.h"

int main (void)
{
	pid_t pid;

	switch (pid = fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:				/* First child */
			switch (fork ()) {
				case -1:
					err_msg ("Can't fork");
					break;

				case 0:		/* Second child */
					sleep (2);
					printf ("In second child; PPID = %ld\n", (long) getppid ());
					_exit (0);

				default:
					_exit (0);
			}
			_exit (0);

		default:			/* Parent */
			if (waitpid (pid, NULL, 0) == -1)
				err_msg ("waitpid failed");
			break;
	}

	return (0);
}
