#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	pid_t pgid;
	char dev [LINE_LEN];

	if (argc != 2)
		err_quit ("Usage: ttypgid terminal");

	snprintf (dev, LINE_LEN, "/dev/%s", argv [1]);

	if ((fd = open (dev, O_RDONLY)) == -1)
		err_msg ("Can't open %s", dev);

	if ((pgid = tcgetpgrp (fd)) == -1)
		err_msg ("tcgetpgrp failed");

	printf ("%s's process group ID is %ld\n", dev, (long) pgid);

	return (0);
}
