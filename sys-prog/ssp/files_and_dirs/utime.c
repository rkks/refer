#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat stat_buf;
	struct utimbuf times;

	if (stat (argv [1], &stat_buf) == -1)
		err_msg ("stat failed");

	if (utime (argv [1], NULL) == -1)
		err_msg ("utime failed");

	sleep (10);

	times.actime = stat_buf.st_atime;
	times.modtime = stat_buf.st_mtime;

	if (utime (argv [1], &times) == -1)
		err_msg ("utime failed");

	return (0);
}
