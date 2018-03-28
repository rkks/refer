#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "ssp.h"

int one_copy (const char *pid_file)
{
	int fd;
	char buf [20];
	int num;

	if ((fd = open (pid_file, O_WRONLY | O_CREAT, FILE_PERMS)) == -1)
		return (-1);

	if (ssp_wlock (fd, 0, SEEK_SET, 0) == -1) {
		if ((errno == EACCES) || (errno == EAGAIN))
			return (1);
		else
			return (-1);
	}

	if (ftruncate (fd, 0) == -1)
		return (-1);

	num = snprintf (buf, sizeof (buf), "%d\n", getpid ());
	if (writen (fd, buf, num) == -1)
		return (-1);

	if ((num = fcntl (fd, F_GETFD, 0)) == -1)
		return (-1);
	num |= FD_CLOEXEC;
	if ((num = fcntl (fd, F_SETFD, 0)) == -1)
		return (-1);

	return (0);
}
