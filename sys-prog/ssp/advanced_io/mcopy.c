#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int src_fd;
	int dest_fd;
	char *src_ptr;
	char *dest_ptr;
	struct stat stat_buf;
	off_t len;

	if (argc != 3)
		err_quit ("Usage: mcopy src dest");

	if ((src_fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open source file %s", argv [1]);

	if (fstat (src_fd, &stat_buf) == -1)
		err_msg ("Can't fstat");
	len = stat_buf.st_size;

	if ((dest_fd = open (argv [2], O_RDWR | O_TRUNC | O_CREAT,
		FILE_PERMS)) == -1) {
		err_msg ("Can't open destination file %s", argv [2]);
	}

	if (ftruncate (dest_fd, len) == -1)
		err_msg ("Can't ftruncate");

	if ((src_ptr = mmap (0, len, PROT_READ, MAP_SHARED,
		src_fd, 0)) == MAP_FAILED) {
		err_msg ("Can't mmap source file");
	}

	if ((dest_ptr = mmap (0, len, PROT_WRITE, MAP_SHARED,
		dest_fd, 0)) == MAP_FAILED) {
		err_msg ("Can't mmap destination file");
	}

	close (src_fd);
	close (dest_fd);

	memcpy (dest_ptr, src_ptr, len);

	return (0);
}
