#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	DIR *dir;
	struct dirent *dirent;

	if (argc != 2)
		err_quit ("Usage: ssp_ls file");

	if ((dir = opendir (argv [1])) == NULL)
		err_msg ("%s: opendir failed", argv [1]);

	while ((dirent = readdir (dir)) != NULL)
		printf ("%s\n", dirent -> d_name);

	closedir (dir);

	return (0);
}
