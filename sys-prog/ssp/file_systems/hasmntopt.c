#include <stdio.h>
#include <sys/mnttab.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	FILE *fp;
	struct mnttab mp;
	int ret;

	if (argc != 2)
		err_quit ("Usage: hasmntopt mount_option");

	if ((fp = fopen ("/etc/mnttab", "r")) == NULL)
		err_msg ("Can't open /etc/mnttab");

	while ((ret = getmntent (fp, &mp)) == 0) {
		if (hasmntopt (&mp, argv [1]))
			printf ("%s\n", mp.mnt_mountp);
	}

	if (ret != -1)
		err_quit ("Bad /etc/mnttab file.\n");

	return (0);
}
