#include <stdio.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct statvfs buf;

	if (argc != 2)
		err_quit ("Usage: statvfs file");

	if (statvfs (argv [1], &buf) == -1)
		err_msg ("statvfs of %s failed", argv [1]);

	printf ("File system containing: %s\n", argv [1]);
	printf ("  Block size: %lu\n", buf.f_bsize);
	printf ("  Fragment size: %lu\n", buf.f_frsize);
	printf ("  Total blocks: %lu\n", buf.f_blocks);
	printf ("  Free blocks: %lu\n", buf.f_bfree);
	printf ("  Available blocks: %lu\n", buf.f_bavail);
	printf ("  Total inodes: %lu\n", buf.f_files);
	printf ("  Free inodes: %lu\n", buf.f_ffree);
	printf ("  Available inodes: %lu\n", buf.f_favail);
	printf ("  File system ID: %lu\n", buf.f_fsid);
	printf ("  File system type: %s\n", buf.f_basetype);

	printf ("  Flags: ");
	if (buf.f_flag == 0)
		printf ("0\n");
	else {
		if (buf.f_flag & ST_RDONLY)
			printf ("ST_RDONLY ");
		if (buf.f_flag & ST_NOSUID)
			printf ("ST_NOSUID ");
		if (buf.f_flag & ST_NOTRUNC)
			printf ("ST_NOTRUNC ");
		printf ("\n");
	}

	printf ("  Maximum filename length: %lu\n", buf.f_namemax);
	printf ("  File system string: %s\n", buf.f_fstr);

	return (0);
}
