#include <stdio.h>
#include <stdlib.h>
#include <sys/mnttab.h>
#include "ssp.h"

static void print_mnttab (struct mnttab *mp);

int main (int argc, char **argv)
{
	FILE *fp;
	struct mnttab mp;
	struct mnttab mpref;
	int ret;
	int i;

	if ((fp = fopen ("/etc/mnttab", "r")) == NULL)
		err_msg ("Can't open /etc/mnttab");

	if (argc == 1) {
		while ((ret = getmntent (fp, &mp)) == 0)
			print_mnttab (&mp);

		if (ret != -1)
			err_quit ("Bad /etc/mnttab file.\n");
	}
	else {
		for (i = 1; argc-- > 1; i++) {
			mpref.mnt_mountp = argv [i];
			mpref.mnt_special = NULL;
			mpref.mnt_fstype = NULL;
			mpref.mnt_mntopts = NULL;
			mpref.mnt_time = NULL;

			switch (getmntany (fp, &mp, &mpref)) {
				case -1:
					rewind (fp);
					break;

				case 0:
					print_mnttab (&mp);
					rewind (fp);
					break;

				default:
					err_quit ("Bad /etc/mnttab file.\n");
					break;
			}
		}
	}

	return (0);
}

static void print_mnttab (struct mnttab *mp)
{
	time_t mount_time;

	mount_time = atol (mp -> mnt_time);
	printf ("Mount point: %s\n", mp -> mnt_mountp);
	printf ("  Mounted from: %s\n", mp -> mnt_special);
	printf ("  File system type: %s\n", mp -> mnt_fstype);
	printf ("  Mount options: %s\n", mp -> mnt_mntopts);
	printf ("  Mount time: %s\n", ctime (&mount_time));
}
