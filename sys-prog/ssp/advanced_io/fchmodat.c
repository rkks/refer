#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

static int fchmodat (int fd, const char *path, mode_t mode);

int main (int argc, char **argv)
{
	mode_t mode;
	int fd;

	if (argc != 4)
		err_quit ("Usage: fchmodat mode file attribute");

	mode = strtol (argv [1], NULL, 8);

	if ((fd = open (argv [2], O_RDONLY)) == -1)
		if ((fd = open (argv [2], O_WRONLY)) == -1)
			err_msg ("Can't open %s", argv [2]);

	if (fchmodat (fd, argv [3], mode) == -1)
		err_msg ("fchmodat failed");

	return (0);
}

static int fchmodat (int fd, const char *path, mode_t mode)
{
	int attr_fd;
	int ret_code;

	if ((attr_fd = openat (fd, path, O_RDONLY | O_XATTR)) == -1)
		if ((attr_fd = openat (fd, path, O_WRONLY | O_XATTR)) == -1)
			return (-1);

	ret_code = fchmod (attr_fd, mode);

	close (attr_fd);

	return (ret_code);
}
