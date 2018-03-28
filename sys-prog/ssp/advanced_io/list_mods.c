#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stropts.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	int i;
	int num_modules;
	struct str_list modules;

	if (argc != 2)
		err_quit ("Usage: list_mods pathname");

	if ((fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open %s", argv [1]);

	if ((num_modules = ioctl (fd, I_LIST, NULL)) == -1)
		err_msg ("Can't get number of modules");

	printf ("Number of modules = %d\n", num_modules);

	if ((modules.sl_modlist = calloc (num_modules,
		sizeof (struct str_mlist))) == NULL) {
		err_msg ("Can't calloc memory");
	}

	modules.sl_nmods = num_modules;

	if (ioctl (fd, I_LIST, &modules) == -1)
		err_msg ("Can't get list of module names");

	for (i = 1; i <= num_modules; i++) {
		if (i == num_modules)
			printf ("  Driver: %s\n", (char *) modules.sl_modlist++);
		else
			printf ("  Module: %s\n", (char *) modules.sl_modlist++);
	}

	close (fd);

	return (0);
}
