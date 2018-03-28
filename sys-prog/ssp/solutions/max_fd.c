#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "ssp.h"

static int max_fd (void);

int main (void)
{
	int max;

	if ((max = max_fd ()) == -1)
		err_msg ("max_fd failed");
	
	printf ("Highest open file descriptor = %d\n", max);

	return (0);
}

static int max_fd (void)
{
	DIR *dirp;
	struct dirent *dir;
	int fd;
	int max;

	max = 0;

	if ((dirp = opendir ("/proc/self/fd")) == NULL)
		return (-1);

	if ((readdir (dirp) == NULL) || (readdir (dirp) == NULL)) {
		closedir (dirp);
		return (-1);
	}

	while ((dir = readdir (dirp)) != NULL) {
		fd = atoi (dir -> d_name);
		if (fd > max)
			max = fd;
	}

	closedir (dirp);

	return (max);
}
