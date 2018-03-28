#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <sys/fs/ufs_fs.h>
#include <sys/fs/ufs_inode.h>
#include <sys/vfstab.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ssp.h"

#define sb sbu.sblock
#define SSP_MAXUID USHRT_MAX	/* Default MAXUID requires too much space! */

union sb_union {
	struct fs sblock;
	char pad [SBSIZE];
};

struct usage {
	uid_t uid;
	size_t blocks;
	size_t inodes;
};

static int diskuse (const char *device, struct usage *use_by_uid);
static void print_results (char *mountp, char *dev, struct usage *use_by_uid);
static int blk_compare (const void *a, const void *b);

int main (int argc, char **argv)
{
	int i;
	int j;
	struct usage use_by_uid [SSP_MAXUID];
	FILE *fp;
	struct vfstab vfstab;

	if (argc < 2)
		err_quit ("Usage: diskuse file_system ...");

	if ((fp = fopen (VFSTAB, "r")) == NULL)
		err_quit ("Can't open %s", VFSTAB);

	for (j = 1; j < argc; j++) {
		rewind (fp);

		if (getvfsfile (fp, &vfstab, argv [j]) == -1) {
			log_msg ("%s: Not found in %s", argv [j], VFSTAB);
			continue;
		}

		memset (use_by_uid, 0, sizeof (use_by_uid));
		for (i = 0; i < SSP_MAXUID; i++)
			use_by_uid [i].uid = i;

		if (diskuse (vfstab.vfs_fsckdev, use_by_uid) == -1)
			continue;

		qsort (use_by_uid, SSP_MAXUID, sizeof (struct usage), blk_compare);

		print_results (vfstab.vfs_mountp, vfstab.vfs_fsckdev, use_by_uid);
	}

	fclose (fp);

	return (0);
}

static int diskuse (const char *device, struct usage *use_by_uid)
{
	ino_t inode;
	daddr32_t inode_blk;
	int i;
	int fd;
	int num_inodes;
	struct dinode inode_tab [MAXBSIZE / sizeof (struct dinode)];
	union sb_union sbu;

	if ((fd = open (device, O_RDONLY)) == -1) {
		err_ret ("diskuse: %s", device);
		return (-1);
	}

	sync ();

	if (pread (fd, &sb, SBSIZE, ldbtob (SBLOCK)) == -1) {
		close (fd);
		return (-1);
	}

	num_inodes = sb.fs_ipg * sb.fs_ncg;

	for (inode = 0; inode < num_inodes;) {
		inode_blk = fsbtodb (&sb, itod (&sb, inode));
		if (pread (fd, inode_tab, sb.fs_bsize, ldbtob (inode_blk)) == -1) {
			close (fd);
			return (-1);
		}

		for (i = 0; i < INOPB (&sb) && inode < num_inodes; i++, inode++) {
			if (inode < UFSROOTINO)
				continue;

			if ((inode_tab [i].di_mode & IFMT) == 0)
				continue;

			use_by_uid [inode_tab [i].di_uid].blocks +=
				inode_tab [i].di_blocks;
 			use_by_uid [inode_tab [i].di_uid].inodes++;
		}
	}

	return (0);
}

static void print_results (char *mountp, char *dev, struct usage *use_by_uid)
{
	int i;
	struct passwd *pwd;

	printf ("%s (%s):\n", mountp, dev);

	for (i = 0; i < SSP_MAXUID; i++) {	
		if (use_by_uid [i].blocks == 0)
			continue;

		if ((pwd = getpwuid (use_by_uid [i].uid)) == NULL)
			printf ("  %-10ld", (long) use_by_uid [i].uid);
		else
			printf ("  %-10s", pwd -> pw_name);

		printf ("%10d", use_by_uid [i].blocks / 2);

		printf ("%10d\n", use_by_uid [i].inodes);
	}

	printf ("\n");
}

static int blk_compare (const void *a, const void *b)
{
	struct usage *x;
	struct usage *y;

	x = (struct usage *) a;
	y = (struct usage *) b;

	return (y -> blocks - x -> blocks);
}
