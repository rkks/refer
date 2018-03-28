#include <stdio.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/mntent.h>
#include <sys/fs/ufs_mount.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char options [MAX_MNTOPT_STR];
	struct ufs_args args;

	if (argc != 3)
		err_quit ("Usage: sspmount special dir");

	snprintf (options, MAX_MNTOPT_STR, "logging");
#ifdef UFSMNT_LARGEFILES
	args.flags = UFSMNT_LARGEFILES;
#else
	args.flags = 0;
#endif

	if (mount (argv [1], argv [2], MS_DATA | MS_NOSUID | MS_OPTIONSTR |
		MS_RDONLY, MNTTYPE_UFS, &args, sizeof (args), options,
		MAX_MNTOPT_STR) == -1) {
		err_msg ("Can't mount %s on %s", argv [1], argv [2]);
	}
	printf ("Options = %s\n", options);

	return (0);
}
