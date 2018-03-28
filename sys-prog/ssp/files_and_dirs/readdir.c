#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>
#include "ssp.h"

static int num_reg;
static int num_dir;
static int num_cspec;
static int num_bspec;
static int num_fifo;
static int num_sock;
static int num_symlink;
static int num_door;

static void process_dir (DIR *dirp, const char *file);

int main (int argc, char **argv)
{
	struct stat stat_buf;
	DIR *dirp;
	int i;

	for (i = 1; i < argc; i++) {
		if (stat (argv [i], &stat_buf) == -1) {
			err_ret ("stat failed: %s", argv [i]);
			continue;
		}

		if (!S_ISDIR (stat_buf.st_mode)) {
			log_msg ("%s: Not a directory", argv [i]);
			continue;
		}

		if ((dirp = opendir (argv [i])) != NULL) {
			process_dir (dirp, argv [i]);
			closedir (dirp);
		}
		else {
			err_ret ("opendir failed: %s", argv [i]);
			continue;
		}

		printf ("Totals for %s:\n", argv [i]);
		printf ("  Regular files: %d\n", num_reg);
		printf ("  Directories: %d\n", num_dir);
		printf ("  Character special files: %d\n", num_cspec);
		printf ("  Block special files: %d\n", num_bspec);
		printf ("  FIFOs: %d\n", num_fifo);
		printf ("  Sockets: %d\n", num_sock);
		printf ("  Symbolic links: %d\n", num_symlink);
		printf ("  Doors: %d\n", num_door);
	}

	return (0);
}

static void process_dir (DIR *dirp, const char *file)
{
	struct stat stat_buf;
	dirent_t *entry;
	char path [MAXPATHLEN];

	num_reg = num_dir = num_cspec = num_bspec = 0;
	num_fifo = num_sock = num_symlink = num_door = 0;

	if ((readdir (dirp) != NULL) && (readdir (dirp) != NULL)) {
		while ((entry = readdir (dirp)) != NULL) {
			snprintf (path, MAXPATHLEN, "%s/%s", file, entry -> d_name);
			if (lstat (path, &stat_buf) == -1) {
				err_ret ("lstat failed: %s", path);
				continue;
			}

			switch (stat_buf.st_mode & S_IFMT) {
				case S_IFREG:
					num_reg++;
					break;

				case S_IFDIR:
					num_dir++;
					break;

				case S_IFCHR:
					num_cspec++;
					break;

				case S_IFBLK:
					num_bspec++;
					break;

				case S_IFIFO:
					num_fifo++;
					break;

				case S_IFSOCK:
					num_sock++;
					break;

				case S_IFLNK:
					num_symlink++;
					break;

				case S_IFDOOR:
					num_door++;
					break;
			}
		}
	}
}
