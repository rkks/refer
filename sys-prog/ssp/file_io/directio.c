#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

int main (void)
{
	char buf [BUF_SIZE];
	ssize_t n;
	int in_fd;
	int out_fd;

	snprintf (buf, BUF_SIZE, "/space/directio");
	if ((in_fd = open (buf, O_RDONLY)) == -1)
		err_msg ("open failed");

	snprintf (buf, BUF_SIZE, "/space/directio.out");
	if ((out_fd = open (buf, O_WRONLY | O_TRUNC | O_CREAT, FILE_PERMS)) == -1)
		err_msg ("open failed");

	if (directio (in_fd, DIRECTIO_OFF) == -1)
		err_msg ("directio failed");
	if (directio (out_fd, DIRECTIO_OFF) == -1)
		err_msg ("directio failed");

	while ((n = read (in_fd, buf, BUF_SIZE)) > 0) {
		if (writen (out_fd, buf, n) == -1)
			err_msg ("writen failed");
	}

	if (n == -1)
		err_msg ("read failed");

	return (0);
}
