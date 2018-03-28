#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "ssp.h"

#define WELL_KNOWN_FIFO "/tmp/ssp_fifo"

static int open_fifo (const char *path);

int main (void)
{
	int fd;

	if ((fd = open_fifo (WELL_KNOWN_FIFO)) == -1)
		err_msg ("Can't open FIFO");

	return (0);
}

static int open_fifo (const char *path)
{
	int read_fd;
	int write_fd;

	if (unlink (path) == -1) {
		if (errno != ENOENT)
			return (-1);
	}

	if (mkfifo (path, FIFO_PERMS) == -1)
		return (-1);

	if ((read_fd = open (path, O_RDONLY | O_NONBLOCK)) == -1)
		return (-1);
	if ((write_fd = open (path, O_WRONLY)) == -1)
		return (-1);

	return (clear_fsflag (read_fd, O_NONBLOCK));
}
