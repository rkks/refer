#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>
#include "ssp.h"

#define CS_PIPE "/tmp/ssp_cs_open"

static int cs_open (char *path, int oflag);

int main (int argc, char **argv)
{
	int fd;
	ssize_t n;
	char buf [BUF_SIZE];

	if (argc != 2)
		err_quit ("Usage: cs_open1 path");

	if ((fd = cs_open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open %s", argv [1]);

	while ((n = read (fd, buf, BUF_SIZE)) > 0)
		if (writen (STDOUT_FILENO, buf, n) == -1)
			err_msg ("writen failed");

	if (n == -1)
		err_msg ("read failed");

	close (fd);

	return (0);
}

static int cs_open (char *path, int oflag)
{
	int len;
	char buf [12];
	struct iovec iov [3];
	int fd;

	if ((fd = cli_connect (CS_PIPE)) == -1)
		err_msg ("cli_connect failed");

	snprintf (buf, 12, " %d", oflag);
	iov [0].iov_base = "open ";
	iov [0].iov_len = strlen ("open ");
	iov [1].iov_base = path;
	iov [1].iov_len = strlen (path);
	iov [2].iov_base = buf;
	iov [2].iov_len = strlen (buf) + 1;

	len = iov [0].iov_len + iov [1].iov_len + iov [2].iov_len;
	if (writev (fd, &iov [0], 3) != len)
		err_msg ("writev failed");

	return (recv_fd (fd));
}
