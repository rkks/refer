#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

static void lock_byte (const char *proc, int fd, int byte);

int main (void)
{
	int fd;

	if ((fd = open ("tmpfile", O_RDWR | O_CREAT | O_TRUNC, FILE_PERMS)) == -1)
		err_msg ("Can't create temp file");

	if (writen (fd, "xx", 2) != 2)
		err_msg ("Write error");

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			lock_byte ("Child", fd, 0);
			sleep (1);
			lock_byte ("Child", fd, 1);
			_exit (0);

		default:
			lock_byte ("Parent", fd, 1);
			sleep (1);
			lock_byte ("Parent", fd, 0);
			break;
	}

	return (0);
}

static void lock_byte (const char *proc, int fd, int byte)
{
	if (ssp_wlockw (fd, SEEK_SET, byte, 1) != 0)
		err_msg ("%s: Can't get write lock", proc);

	printf ("%s: Got a lock on byte %d\n", proc, byte);
}
