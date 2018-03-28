#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include "ssp.h"

int main (void)
{
	char *cwd;

	if (chdir ("/lib/nfs") != 0)
		err_msg ("chdir failed");

	if ((cwd = getcwd (NULL, MAXPATHLEN)) == NULL) {
		err_msg ("getcwd failed");
		exit (1);
	}

	printf ("CWD = %s\n", cwd);

	free (cwd);

	return (0);
}
