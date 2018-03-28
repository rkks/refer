#include <stdio.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	struct stat buf;

	if (argc != 2)
		err_quit ("Usage: ftok pathname");

	if (stat (argv [1], &buf) == -1)
		err_msg ("stat failed");

	printf ("st_dev = %lx, st_ino = %lx, key = %x\n", (long) buf.st_dev,
		(long) buf.st_ino, ftok (argv [1], 0x42));

	return (0);
}
