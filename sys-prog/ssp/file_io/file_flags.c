#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

extern void print_fsflags (int flags);

int main (int argc, char **argv)
{
	int flags;
	int fd;

	if (argc != 2)
		err_quit ("Usage: file_flags <file descriptor>");

	if ((fd = atoi (argv [1])) == -1)
		err_msg ("atoi failed");

	if ((flags = fcntl (fd, F_GETFL)) == -1)
		err_msg ("Can't get file status flags");

	printf ("Flags for FD %d = ", fd);
	print_fsflags (flags);

	return (0);
}
