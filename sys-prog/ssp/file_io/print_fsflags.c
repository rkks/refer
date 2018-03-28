#include <stdio.h>
#include <fcntl.h>
#include "ssp.h"

void print_fsflags (int flags)
{
	switch (flags & O_ACCMODE) {
		case O_RDONLY:
			printf ("O_RDONLY");
			break;

		case O_WRONLY:
			printf ("O_WRONLY");
			break;

		case O_RDWR:
			printf ("O_RDWR");
			break;

		default:
			err_dump ("Unrecognised file status flag: %d", flags & O_ACCMODE);
	}

	if (flags & O_APPEND)
		printf (", O_APPEND");
	if (flags & O_CREAT)
		printf (", O_CREAT");
	if (flags & O_DSYNC)
		printf (", O_DSYNC");
	if (flags & O_EXCL)
		printf (", O_EXCL");
#ifdef O_LARGEFILE
	if (flags & O_LARGEFILE)
		printf (", O_LARGEFILE");
#endif
	if (flags & O_NOCTTY)
		printf (", O_NOCTTY");
	if (flags & O_NDELAY)
		printf (", O_NDELAY");
	if (flags & O_NONBLOCK)
		printf (", O_NONBLOCK");
	if (flags & O_RSYNC)
		printf (", O_RSYNC");
	if (flags & O_SYNC)
		printf (", O_SYNC");
	if (flags & O_TRUNC)
		printf (", O_TRUNC");
#ifdef O_XATTR
	if (flags & O_XATTR)
		printf (", O_XATTR");
#endif

	printf ("\n");
}
