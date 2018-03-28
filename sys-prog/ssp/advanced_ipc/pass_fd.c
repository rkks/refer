#include <unistd.h>
#include <errno.h>
#include <stropts.h>
#include "ssp.h"

int send_fd (int pipe_fd, int fd)
{
	char buf;

	buf = 0;

	if (writen (pipe_fd, &buf, 1) != 1)
		return (-1);

	if (ioctl (pipe_fd, I_SENDFD, fd) == -1)
		return (-1);

	return (0);
}

int send_err (int pipe_fd, char status)
{
	if (status == 0)
		status = EINVAL;

	return (writen (pipe_fd, &status, 1));
}

int recv_fd (int pipe_fd)
{
	int new_fd;
	ssize_t n;
	char buf;
	struct strrecvfd recvfd;

	if ((n = read (pipe_fd, &buf, 1)) == -1)
		err_msg ("read failed");

	if (n == 0) {
		err_ret ("Connection close by server");
		new_fd = -1;
	}
	else {
		if (buf == 0) {
			if (ioctl (pipe_fd, I_RECVFD, &recvfd) == -1)
				return (-1);
			new_fd = recvfd.fd;
		}
		else {
			errno = buf;
			new_fd = -1;
		}
	}

	return (new_fd);
}
