#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/mntent.h>
#include <sys/fs/ufs_mount.h>
#include "ssp.h"

#define SPEC "/dev/dsk/c0t0d0s7"
#define DIR "/space"

int main (int argc, char **argv)
{
	int fd;
	char *buf1 = "This was written before the mount\n";
	char *buf2 = "This was written after the mount\n";
	struct ufs_args args;

	if ((fd = open ("/space/foo", O_WRONLY | O_CREAT, FILE_PERMS)) == -1)
		err_msg ("Can't creat foo");

	if (writen (fd, buf1, strlen (buf1)) == -1)
		err_msg ("writen failed");

#ifdef UFSMNT_LARGEFILES
	args.flags = UFSMNT_LARGEFILES;
#else
	args.flags = 0;
#endif

	if (mount (SPEC, DIR, MS_DATA, MNTTYPE_UFS, &args, sizeof (args),
		NULL, 0) == -1) {
		err_msg ("mount failed");
	}

	if (writen (fd, buf2, strlen (buf2)) == -1)
		err_msg ("writen failed");

	close (fd);

	return (0);
}
