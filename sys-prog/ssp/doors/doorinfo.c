#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	struct stat stat_buf;
	door_info_t info;

	if (argc != 2)
		err_quit ("Usage: doorinfo path");

	if ((fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open door");

	if (fstat (fd, &stat_buf) == -1)
		err_msg ("stat failed");
	if (S_ISDOOR (stat_buf.st_mode) == 0)
		err_quit ("%s: Not a door", argv [1]);

	if (door_info (fd, &info) == -1)
		err_msg ("door_info failed");

	printf ("Door info:\n");
	printf ("  Server process ID = %ld\n", (long) info.di_target);
	printf ("  Uniquifier = %lld\n", info.di_uniquifier);
	printf ("  Attributes =");
	if (info.di_attributes == 0)
		printf (" 0");
	else {
		if (info.di_attributes & DOOR_LOCAL)
			printf (" DOOR_LOCAL");
		if (info.di_attributes & DOOR_UNREF)
			printf (" DOOR_UNREF");
#ifdef DOOR_UNREF_MULTI
		if (info.di_attributes & DOOR_UNREF_MULTI)
			printf (" DOOR_UNREF_MULTI");
		if (info.di_attributes & DOOR_IS_UNREF)
			printf (" DOOR_IS_UNREF");
#endif
		if (info.di_attributes & DOOR_REVOKED)
			printf (" DOOR_REVOKED");
		if (info.di_attributes & DOOR_PRIVATE)
			printf (" DOOR_PRIVATE");
	}
	printf ("\n");

	return (0);
}
