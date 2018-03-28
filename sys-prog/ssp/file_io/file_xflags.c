#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

extern void print_fsflags (int flags);

int main (int argc, char **argv)
{
	int flags;
	int fd;

	if ((fd = open ("/tmp/rich", O_RDONLY | O_APPEND | O_DSYNC | O_RSYNC |
		O_SYNC | O_CREAT | O_TRUNC | O_LARGEFILE, FILE_PERMS)) == -1) {
		err_msg ("Open failed");
	}

	if ((flags = fcntl (fd, F_GETFL)) == -1)
		err_msg ("Can't get file status flags");

	printf ("Flags for FD %d = ", fd);
	print_fsflags (flags);

	if ((flags = fcntl (fd, F_GETXFL)) == -1)
		err_msg ("Can't get extended file status flags");

	printf ("Extended flags for FD %d = ", fd);
	print_fsflags (flags);

	return (0);
}
