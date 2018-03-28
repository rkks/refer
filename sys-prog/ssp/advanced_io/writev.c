#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include "ssp.h"

static void write_buf (int fd, char *buf, int buf_size);

int main (void)
{
	int fd;
	char buf [256];
	int i;

	if ((fd = open ("/tmp/test", O_RDWR | O_CREAT | O_TRUNC,
		FILE_PERMS)) == -1) {
		err_msg ("Can't open temp file");
	}

	for (i = 0; i < 100000; i++)
		write_buf (fd, buf, sizeof (buf));

	close (fd);
	return (0);
}

static void write_buf (int fd, char *buf, int buf_size)
{
	struct iovec iov [2];

	iov [0].iov_base = (char *) &buf_size;
	iov [0].iov_len = sizeof (int);
	iov [1].iov_base = buf;
	iov [1].iov_len = buf_size;

	if (writev (fd, iov, 2) != sizeof (int) + buf_size)
		err_msg ("writev failed");
}
