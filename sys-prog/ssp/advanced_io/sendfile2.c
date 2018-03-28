#include <stdio.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

static char buf [LINE_LEN];

int main (int argc, char **argv)
{
	int out_fd;
	off_t off;
	ssize_t len;
	ssize_t res;

	if (argc != 2)	
		err_quit ("Usage: sendfile2 dest_file");

	if ((out_fd = open (argv [1], O_WRONLY | O_TRUNC | O_CREAT,
		FILE_PERMS)) == -1) {
		err_msg ("Can't open %s", argv [1]);
	}

	len = snprintf (buf, LINE_LEN, "Hello, World!\n");

	off = (off_t) buf;
	if ((res = sendfile (out_fd, SFV_FD_SELF, &off, len)) == -1)
		err_msg ("sendfile failed");
	else
		printf ("Copied %d bytes\n", res);

	close (out_fd);

	return (0);
}
