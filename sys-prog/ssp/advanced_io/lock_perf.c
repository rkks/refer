#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "ssp.h"

int main (void)
{
	int fd1;
	int fd2;
	char buf [1024];
	int status;

	if ((fd1 = open ("tmpfile", O_RDONLY, 0)) == -1)
		err_msg ("Can't open temp file");

	if (ssp_rlockw (fd1, SEEK_SET, 0, 0) != 0)
		err_msg ("Can't get read lock");

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			if ((fd2 = open ("tmpfile.out", O_RDWR | O_CREAT | O_TRUNC,
				FILE_PERMS)) == -1) {
				err_msg ("Can't open output file");
			}

			while (read (fd1, buf, 1024) != 0)
				writen (fd2, buf, 1024);

			_exit (0);

		default:
			wait (&status);
			close (fd1);
			break;
	}

	return (0);
}
