#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mkdev.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat buf;
	int i;

	for (i = 1; i < argc; i++) {
		if (stat (argv [i], &buf) == -1) {
			err_ret ("stat failed: %s", argv [i]);
			continue;
		}

		printf ("%s: st_dev = %ld, %ld", argv [i],
			(long) major (buf.st_dev), (long) minor (buf.st_dev));

		if ((S_ISCHR (buf.st_mode)) || (S_ISBLK (buf.st_mode)))
			printf (" (st_rdev = %ld, %ld)",
				(long) major (buf.st_rdev), (long) minor (buf.st_rdev));

		printf ("\n");
	}

	return (0);
}
