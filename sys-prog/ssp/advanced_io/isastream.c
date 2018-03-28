#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	int i;

	if (argc < 2)
		err_quit ("Usage: isastream file ...");

	for (i = 1; i < argc; i++) {
		if ((fd = open (argv [i], O_RDONLY)) == -1) {
			err_ret ("Can't open %s", argv [i]);
			continue;
		}

		printf ("%s %s a STREAMS device\n", argv [i],
			(isastream (fd)) ? "is" : "is not");
	}

	return (0);
}
