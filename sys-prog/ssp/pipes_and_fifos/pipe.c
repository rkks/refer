#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "ssp.h"

int main (void)
{
	int fd [2];
	char buf [LINE_LEN];
	ssize_t n;

	if (pipe (fd) == -1)
		err_msg ("pipe failed");

	switch (fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			close (fd [1]);
			n = read (fd [0], buf, LINE_LEN);
			writen (STDOUT_FILENO, "Child reads: ", 13);
			writen (STDOUT_FILENO, buf, 15);
			_exit (0);

		default:
			close (fd [0]);
			writen (fd [1], "Hello, World!\n", 15);
			wait (NULL);
			break;
	}

	return (0);
}
