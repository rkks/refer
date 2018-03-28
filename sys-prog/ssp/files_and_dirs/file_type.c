#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat buf;
	char *type;
	int i;

	for (i = 1; i < argc; i++) {
		if (lstat (argv [i], &buf) == -1) {
			err_ret ("lstat failed: %s", argv [i]);
			continue;
		}
		printf ("%s is a ", argv [i]);

		if (S_ISREG (buf.st_mode))
			type = "regular file";
		if (S_ISDIR (buf.st_mode))
			type = "directory";
		if (S_ISCHR (buf.st_mode))
			type = "character special file";
		if (S_ISBLK (buf.st_mode))
			type = "block special file";
		if (S_ISFIFO (buf.st_mode))
			type = "FIFO";
		if (S_ISSOCK (buf.st_mode))
			type = "socket";
		if (S_ISLNK (buf.st_mode))
			type = "symbolic link";
#ifdef S_ISDOOR
		if (S_ISDOOR (buf.st_mode))
			type = "door";
#endif

		printf ("%s\n", type);
	}

	return (0);
}
