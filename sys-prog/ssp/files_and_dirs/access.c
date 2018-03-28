#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;

	if (argc != 2)
		err_quit ("Usage: access filename");

	if ((access (argv [1], R_OK)) == 0)
		printf ("Read access OK\n");
	else
		err_ret ("Read access denied");

	if ((fd = open (argv [1], O_RDONLY)) == -1)
		err_ret ("Read only open failed");
	else {
		printf ("Read only open OK\n");
		close (fd);
	}

	return (0);
}
