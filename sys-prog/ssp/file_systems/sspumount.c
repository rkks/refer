#include <stdio.h>
#include <sys/mount.h>
#include <errno.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char resp;

	if (argc != 2)
		err_quit ("Usage: sspumount dir");

	if (umount2 (argv [1], 0) == -1) {
		if (errno == EBUSY) {
			printf ("The file system mounted on %s is busy.\n", argv [1]);
			printf ("Do you want to force the unmount?  ");
			resp = fgetc (stdin);
			if (resp == 'y') {
				if (umount2 (argv [1], MS_FORCE) == -1)
					err_msg ("Forced umount failed");
			}
		}
		else
			err_msg ("umount failed");
	}

	return (0);
}
