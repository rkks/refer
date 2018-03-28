#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mkdev.h>
#include <sys/param.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "ssp.h"

static void print_formatted (struct stat *buf, char *path);
static void print_type (int type, char *path);
static void print_perms (int perms);
static void print_raw (struct stat *buf);

int main (int argc, char **argv)
{
	char arg_char;
	boolean_t err_flag;
	boolean_t raw_flag;
	struct stat buf;

	opterr = 0;
	err_flag = B_FALSE;
	raw_flag = B_FALSE;

	while ((arg_char = getopt (argc, argv, "r")) != EOF) {
		switch (arg_char) {
			case 'r':
				raw_flag = B_TRUE;
				break;

			default:
				err_flag = B_TRUE;
				break;
		}
	}

	if (err_flag)
		err_quit ("Usage: stat [-r] file ...");

	while (optind != argc) {
		if (lstat (argv [optind], &buf) != 0) {
			err_ret ("stat: Can't lstat %s", argv [optind]);
			continue;
		}

		printf ("%s:\n", argv [optind]);
		if (raw_flag)
			print_raw (&buf);
		else
			print_formatted (&buf, argv [optind]);
		optind++;
	}

	return (0);
}

static void print_formatted (struct stat *buf, char *path)
{
	struct passwd *pwd;
	struct group *grp;

	printf ("  File system device: %ld, %ld\n",
		(long) major (buf -> st_dev), (long) minor (buf -> st_dev));
	printf ("  Inode: %lld\n", (long long) buf -> st_ino);
	print_type (buf -> st_mode & S_IFMT, path);
	print_perms (buf -> st_mode);
	printf ("  Number of links: %ld\n", (long) buf -> st_nlink);

	if ((pwd = getpwuid (buf -> st_uid)) == NULL)
		printf ("  User ID: %ld\n", (long) buf -> st_uid);
	else
		printf ("  User ID: %s\n", pwd -> pw_name);

	if ((grp = getgrgid (buf -> st_gid)) == NULL)
		printf ("  Group ID: %ld\n", (long) buf -> st_gid);
	else
		printf ("  Group ID: %s\n", grp -> gr_name);

	if ((S_ISCHR (buf -> st_mode)) || (S_ISBLK (buf -> st_mode)))
		printf ("  Special file device numbers: %ld, %ld\n",
			(long) major (buf -> st_rdev), (long) minor (buf -> st_dev));

	printf ("  File size: %lld\n", (long long) buf -> st_size);
	printf ("  Last access time: %s", ctime (&(buf -> st_atime)));
	printf ("  Last modification time: %s", ctime (&(buf -> st_mtime)));
	printf ("  Last inode change: %s", ctime (&(buf -> st_ctime)));
	printf ("  Preferred block size: %ld\n", (long) buf -> st_blksize);
	printf ("  Number of blocks: %ld\n", (long) buf -> st_blocks);
	printf ("  File system type: %s\n", buf -> st_fstype);
}

static void print_type (int type, char *path)
{
	char link_buf [MAXPATHLEN];
	int n;

	printf ("  File type: ");
	switch (type) {
		case S_IFREG:
			printf ("Regular file\n");
			break;

		case S_IFDIR:
			printf ("Directory\n");
			break;

		case S_IFCHR:
			printf ("Character special\n");
			break;

		case S_IFBLK:
			printf ("Block special\n");
			break;

		case S_IFIFO:
			printf ("FIFO\n");
			break;

		case S_IFSOCK:
			printf ("Socket\n");
			break;

		case S_IFLNK:
			if ((n = (readlink (path, link_buf, MAXPATHLEN))) == -1)
				snprintf (link_buf, MAXPATHLEN, "<Indeterminate>");
			else
				link_buf [n] = '\0';
			printf ("Symbolic link to %s\n", link_buf);
			break;

		case S_IFDOOR:
			printf ("Door\n");
			break;

		default:
			printf ("Unrecognised\n");
			break;
	}
}

static void print_perms (int perms)
{
	printf ("  Permissions: ");

	if (perms & S_IRUSR)
		printf ("r");
	else
		printf ("-");
	if (perms & S_IWUSR)
		printf ("w");
	else
		printf ("-");
	if (perms & S_IXUSR)
		printf ("x");
	else
		printf ("-");

	if (perms & S_IRGRP)
		printf ("r");
	else
		printf ("-");
	if (perms & S_IWGRP)
		printf ("w");
	else
		printf ("-");
	if (perms & S_IXGRP)
		printf ("x");
	else
		printf ("-");

	if (perms & S_IROTH)
		printf ("r");
	else
		printf ("-");
	if (perms & S_IWOTH)
		printf ("w");
	else
		printf ("-");
	if (perms & S_IXOTH)
		printf ("x");
	else
		printf ("-");

	if (perms & S_ISUID)
		printf (", set UID");
	if (perms & S_ISGID) {
		if (perms & S_IXGRP)
			printf (", set GID");
		else
			printf (", mandatory locking");
	}

	if (perms & S_ISVTX)
		printf (", sticky bit");

	printf ("\n");
}

static void print_raw (struct stat *buf)
{
	printf ("  st_dev: %ld\n", (long) buf -> st_dev);
	printf ("  st_ino: %lld\n", (long long) buf -> st_ino);
	printf ("  st_mode: %ld\n", (long) buf -> st_mode);
	printf ("  st_nlink: %ld\n", (long) buf -> st_nlink);
	printf ("  st_uid: %ld\n", (long) buf -> st_uid);
	printf ("  st_gid: %ld\n", (long) buf -> st_gid);
	printf ("  st_rdev: %ld\n", (long) buf -> st_rdev);
	printf ("  st_size: %lld\n", (long long) buf -> st_size);
	printf ("  st_atime: %ld\n", buf -> st_atime);
	printf ("  st_mtime: %ld\n", buf -> st_mtime);
	printf ("  st_ctime: %ld\n", buf -> st_ctime);
	printf ("  st_blksize: %ld\n", (long) buf -> st_blksize);
	printf ("  st_blocks: %ld\n", (long) buf -> st_blocks);
	printf ("  st_fstype: %s\n", buf -> st_fstype);
}
