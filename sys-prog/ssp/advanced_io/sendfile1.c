#include <stdio.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat buf;
	int in_fd;
	int out_fd;
	off_t off;
	ssize_t res;

	if (argc != 3)	
		err_quit ("Usage: sendfile1 source_file dest_file");

	if ((in_fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open %s", argv [1]);
	if (fstat (in_fd, &buf) == -1)
		err_msg ("Can't stat %s", argv [1]);

	if ((out_fd = open (argv [2], O_WRONLY | O_TRUNC | O_CREAT,
		FILE_PERMS)) == -1) {
		err_msg ("Can't open %s", argv [2]);
	}

	off = 0;
	if ((res = sendfile (out_fd, in_fd, &off, buf.st_size)) == -1)
		err_msg ("sendfile failed");
	else
		printf ("Copied %d bytes\n", res);

	close (in_fd);
	close (out_fd);

	return (0);
}
