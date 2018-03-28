#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

static char *path;
static int oflag;

static void do_request (char *buf, ssize_t n, int pipe_fd);
static int buf2args (char *buf);
static int parse_args (int argc, char **argv);

int main (void)
{
	ssize_t n;
	char buf [LINE_LEN];

	for (;;) {
		if ((n = read (STDIN_FILENO, buf, LINE_LEN)) == -1)
			err_msg ("read failed");
		if (n == 0)
			break;

		do_request (buf, n, STDOUT_FILENO);
	}

	return (0);
}

static void do_request (char *buf, ssize_t n, int pipe_fd)
{
	int fd;

	if (buf [n - 1] != '\0') {
		send_err (pipe_fd, EINVAL);
		return;
	}

	if (buf2args (buf) == -1) {
		send_err (pipe_fd, EINVAL);
		return;
	}

	if ((fd = open (path, oflag)) == -1) {
		send_err (pipe_fd, errno);
		return;
	}

	if (send_fd (pipe_fd, fd) == -1)
		err_msg ("send_fd failed");

	close (fd);
}

static int buf2args (char *buf)
{
	char *ptr;
	char *argv [_POSIX_ARG_MAX];
	int argc;

	if (strtok (buf, " \t\n") == NULL)
		return (-1);

	argc = 0;
	argv [argc] = buf;
	while ((ptr = strtok (NULL, " \t\n")) != NULL) {
		if (++argc >= (_POSIX_ARG_MAX - 1))
			return (-1);
		argv [argc] = ptr;
	}
	argv [++argc] = NULL;

	return (parse_args (argc, argv));
}

static int parse_args (int argc, char **argv)
{
	if ((argc != 3) || (strcmp (argv [0], "open") != 0))
		return (-1);

	path = argv [1];
	oflag = atoi (argv [2]);

	return (0);
}
