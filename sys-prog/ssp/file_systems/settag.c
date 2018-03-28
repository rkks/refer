#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mnttab.h>
#include <sys/stat.h>
#include <sys/mntio.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	FILE *fp;
	int fd;
	struct extmnttab mp;
	struct mnttagdesc dp;
	int ret;

	if (argc != 3)
		err_quit ("Usage: settag filsystem tag");

	if ((fp = fopen ("/etc/mnttab", "r")) == NULL)
		err_msg ("Can't open /etc/mnttab");

	fd = fileno (fp);

	while ((ret = getextmntent (fp, &mp, 0)) == 0) {
		if (strcmp (mp.mnt_mountp, argv [1]) == 0) {
			dp.mtd_major = mp.mnt_major;
			dp.mtd_minor = mp.mnt_minor;
			dp.mtd_mntpt = mp.mnt_mountp;
			dp.mtd_tag = argv [2];
			if (ioctl (fd, MNTIOC_SETTAG, &dp) == -1)
				err_msg ("ioctl failed");
			break;
		}
	}

	if (ret > 0)
		err_quit ("Bad /etc/mnttab file.\n");

	return (0);
}
