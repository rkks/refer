#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ustat.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat sbuf;
	struct ustat buf;

	if (argc != 2)
		err_quit ("Usage: ustat file");

	if (stat (argv [1], &sbuf) == -1)
		err_msg ("stat of %s failed", argv [1]);
	if (ustat (sbuf.st_dev, &buf) == -1)
		err_msg ("ustat of %s failed", argv [1]);

	printf ("File system containing: %s\n", argv [1]);
	printf ("  Total free blocks: %lu\n", buf.f_tfree);
	printf ("  Total free inodes: %lu\n", buf.f_tinode);
	printf ("  File system name: %s\n", buf.f_fname);
	printf ("  File system pack name: %s\n", buf.f_fpack);

	return (0);
}
