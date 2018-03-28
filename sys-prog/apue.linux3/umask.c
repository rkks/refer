#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	"ourhdr.h"

int
main(void)
{
	umask(0);
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
										 S_IROTH | S_IWOTH) < 0)
		err_sys("creat error for foo");

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
										 S_IROTH | S_IWOTH) < 0)
		err_sys("creat error for bar");
	exit(0);
}
