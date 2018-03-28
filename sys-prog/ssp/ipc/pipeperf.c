#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ssp.h"

#define BUFFER_SIZE 2000
#define NUM_ITERS 50000

int main (int argc, char **argv)
{
	int i;
	pid_t pid;
	int fd [2];
	char *buf;

	if (argc != 1)
		err_quit ("Usage: pipeperf");

	if (pipe (fd) == -1)
		err_msg ("pipe failed");

	if ((buf = malloc (BUFFER_SIZE)) == NULL)
		err_msg ("malloc failed");

	switch (pid = fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			close (fd [1]);
			for (;;)
				if (read (fd [0], buf, BUFFER_SIZE) == -1)
					err_msg ("read failed");

		default:
			close (fd [0]);
			for (i = 0; i < NUM_ITERS; i++)
				if (writen (fd [1], buf, BUFFER_SIZE) == -1)
					err_msg ("writen failed");
			kill (pid, SIGINT);
			waitpid (pid, NULL, 0);
			break;
	}

	free (buf);

	return (0);
}
