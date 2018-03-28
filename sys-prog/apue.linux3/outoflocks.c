#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	"ourhdr.h"

int
main(void)
{
	int		i, fd;

	if ( (fd = open("temp.lock", O_RDWR | O_CREAT | O_TRUNC,
												FILE_MODE)) < 0)
		err_sys("open error");

	for (i = 0; i < 1000000; i++) {	/* try to write 2 Mbytes */
				/* lock from current EOF to EOF */
		if (writew_lock(fd, 0, SEEK_END, 0) < 0)
			err_sys("writew_lock error");

		if (write(fd, &fd, 1) != 1)
			err_sys("write error");

		if (un_lock(fd, 0, SEEK_END, 0) < 0)
			err_sys("un_lock error");

		if (write(fd, &fd, 1) != 1)
			err_sys("write error");
	}
	exit(0);
}
