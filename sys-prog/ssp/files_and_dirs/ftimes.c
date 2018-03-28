#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat stat_buf;

	if (stat (argv [1], &stat_buf) == -1)
		err_msg ("stat failed");

	printf ("st_atime = %s", ctime (&stat_buf.st_atime));
	printf ("st_mtime = %s", ctime (&stat_buf.st_mtime));
	printf ("st_ctime = %s", ctime (&stat_buf.st_ctime));

	return (0);
}
