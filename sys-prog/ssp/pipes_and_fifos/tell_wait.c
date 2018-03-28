#include <unistd.h>
#include <sys/types.h>
#include "ssp.h"

static int pipe1 [2];
static int pipe2 [2];

int ssp_tell_wait_init (void)
{
	if ((pipe (pipe1) == -1) || (pipe (pipe2) == -1)) {
		close (pipe1 [0]);
		close (pipe1 [1]);
		return (-1);
	}
	else
		return (0);
}

int ssp_tell_child (pid_t pid)
{
	return (writen (pipe1 [1], "p", 1));
}

int ssp_tell_parent (pid_t pid)
{
	return (writen (pipe2 [1], "c", 1));
}

int ssp_wait_child (void)
{
	char c;

	if (read (pipe2 [0], &c, 1) == -1)
		return (-1);

	return ((c == 'c') ? 0 : -1);
}

int ssp_wait_parent (void)
{
	char c;

	if (read (pipe1 [0], &c, 1) == -1)
		return (-1);

	return ((c == 'p') ? 0 : -1);
}

int ssp_tell_wait_fin (void)
{
	close (pipe1 [0]);
	close (pipe1 [1]);
	close (pipe2 [0]);
	close (pipe2 [1]);

	return (0);
}
