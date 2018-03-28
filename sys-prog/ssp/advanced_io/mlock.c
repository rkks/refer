#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	char *buf;
	struct stat stat_buf;
	off_t len;

	if (argc != 2)
		err_quit ("Usage: mlock file");

	if ((fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open %s", argv [1]);

	if (fstat (fd, &stat_buf) == -1)
		err_msg ("Can't fstat");
	len = stat_buf.st_size;

	if ((buf = mmap (0, len, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
		err_msg ("Can't mmap file");

	if (mlock (buf, len) == -1)
		err_msg ("Can't lock file in memory");

	pause ();

	return (0);
}
