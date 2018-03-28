#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "ssp.h"

int main (void)
{
	int pipe_fd [2];
	int i;
	int n;
	struct pollfd fds;

	if (pipe (pipe_fd) == -1)
		err_msg ("pipe failed");

	fds.fd = pipe_fd [1];
	fds.events = POLLOUT;
	fds.revents = 0;

	for (i = 0;; i++) {
		if ((n = poll (&fds, 1, 0)) == -1)
			err_msg ("poll failed");
		if (n == 0)
			break;
		if (writen (pipe_fd [1], "x", 1) == -1)
			err_msg ("writen failed");
	}

	printf ("Pipe capacity = %d bytes\n", i);

	return (0);
}
