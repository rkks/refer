#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mount.h>
#include "ssp.h"

#define DIR "/space"

int main (int argc, char **argv)
{
	int fd;
	char *buf1 = "This was written before the unmount\n";
	char *buf2 = "This was written after the unmount\n";

	if ((fd = open ("/space/foo", O_WRONLY | O_CREAT, FILE_PERMS)) == -1)
		err_msg ("Can't creat foo");

	if (writen (fd, buf1, strlen (buf1)) == -1)
		err_msg ("writen failed");

	if (umount (DIR) == -1)
		err_ret ("umount failed");
#ifdef MS_FORCE
	log_msg ("Attempting to force");
	if (umount2 (DIR, MS_FORCE) == -1)
		err_msg ("umount2 failed");
#endif

	if (writen (fd, buf2, strlen (buf2)) == -1)
		err_msg ("writen failed");

	close (fd);

	return (0);
}
