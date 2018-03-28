#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

int main (void)
{
	int fd;

	if ((fd = open ("/tmp/test_file", O_RDONLY)) == -1)
		err_msg ("Can't open test_file");

	unlink ("/tmp/test_file");

	sleep (10);

	printf ("Finished\n");
	close (fd);

	return (0);
}
