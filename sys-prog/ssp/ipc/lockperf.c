#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ssp.h"

#define NUM_ITERS 50000

static void do_ops (int fd);

int main (int argc, char **argv)
{
	pid_t pid;
	int fd;
	FILE *fp;

	if (argc != 1)
		err_quit ("Usage: lockperf");

	if ((fp = tmpfile ()) == NULL)
		err_msg ("tmpfile failed");
	fd = fileno (fp);

	switch (pid = fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			do_ops (fd);
			_exit (0);

		default:
			do_ops (fd);
			waitpid (pid, NULL, 0);
			break;
	}

	return (0);
}

static void do_ops (int fd)
{
	int i;

	for (i = 0; i < NUM_ITERS; i++) {
		if (lockf (fd, F_LOCK, 0) == -1)
			err_msg ("lockf failed");
		if (lockf (fd, F_ULOCK, 0) == -1)
			err_msg ("lockf failed");
	}
}
