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
	off_t size;

	if ((fd = open (BIG_FILE, O_RDONLY)) == -1)
		err_msg ("Can't open %s", BIG_FILE);

	if ((size = lseek (fd, 0, SEEK_END)) == -1)
		err_msg ("lseek failed");

	printf ("File size is %ld bytes\n", size);
	close (fd);

	return (0);
}
