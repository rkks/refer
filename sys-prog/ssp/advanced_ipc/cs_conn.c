#include <unistd.h>
#include <stropts.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

int srv_listen (const char *path)
{
	int fd;
	int pipe_fd [2];

	unlink (path);
	if ((fd = creat (path, FIFO_PERMS)) == -1)
		return (-1);
	close (fd);

	if (pipe (pipe_fd) == -1)
		return (-1);
	if (ioctl (pipe_fd [1], I_PUSH, "connld") == -1)
		return (-1);

	if (fattach (pipe_fd [1], path) == -1)
		return (-1);

	return (pipe_fd [0]);
}

int srv_accept (int fd, uid_t *uid)
{
	struct strrecvfd recvfd;

	if (ioctl (fd, I_RECVFD, &recvfd) == -1)
		return (-1);

	if (uid != NULL)
		*uid = recvfd.uid;

	return (recvfd.fd);
}

int cli_connect (const char *path)
{
	int fd;

	if ((fd = open (path, O_RDWR)) == -1)
		return (-1);

	return ((isastream (fd)) ? fd : -1);
}
