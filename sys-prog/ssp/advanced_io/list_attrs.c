#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	int i;
	DIR *dirp;
	struct dirent *dir;
	boolean_t no_attrs;

	if (argc < 2)
		err_quit ("Usage: list_attrs file ...");

	for (i = 1; i < argc; i++) {
		if ((fd = attropen (argv [i], ".", O_RDONLY)) == -1) {
			err_msg ("list_attrs: %s", argv [i]);
			continue;
		}

		dirp = fdopendir (fd);

		printf ("%s:", argv [i]);

		no_attrs = B_TRUE;

		if ((readdir (dirp) == NULL) || (readdir (dirp) == NULL)) {
			goto bail;
		}

		while ((dir = readdir (dirp)) != NULL) {
			no_attrs = B_FALSE;
			printf (" %s", dir -> d_name);
		}

bail:
		if (no_attrs)
			printf (" Has no extended attributes\n");
		else
			printf ("\n");

		closedir (dirp);
	}

	return (0);
}
