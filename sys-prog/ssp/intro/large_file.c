#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

#define BIG_FILE "/space/big_file"

int main (void)
{
	int fd;

	if ((fd = open (BIG_FILE, O_RDONLY)) == -1)
		err_msg ("Can't open %s", BIG_FILE);

	printf ("Open successful\n");
	close (fd);

	return (0);
}
