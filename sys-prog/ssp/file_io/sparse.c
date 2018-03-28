#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ssp.h"

int main (void)
{
	int fd;
	char buf1 [] = "DEADBEEF";
	char buf2 [] = "C0FFEE12";

	if ((fd = creat ("sparse_file", FILE_PERMS)) == -1)
		err_msg ("creat error");

	if ((write (fd, buf1, 8)) != 8)
		err_msg ("buf1 write error");

	if ((lseek (fd, 36, SEEK_SET)) == -1)
		err_msg ("lseek error");

	if ((write (fd, buf2, 8)) != 8)
		err_msg ("buf2 write error");

	close (fd);

	return (0);
}
