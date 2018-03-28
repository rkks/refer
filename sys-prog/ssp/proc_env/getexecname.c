#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "ssp.h"

int main (void)
{
	char *cwd;
	const char *path;

	if ((path = getexecname ()) == NULL)
		err_msg ("getexecname failed");

	if (*path != '/') {
		if ((cwd = getcwd (NULL, PATH_MAX)) == NULL)
			err_msg ("getcwd failed");

		printf ("exec name = %s/%s\n", cwd, path);
		free (cwd);
	}
	else
		printf ("exec name = %s\n", path);

	return (0);
}
