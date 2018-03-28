#include <stdio.h>
#include <sys/types.h>
#include <ftw.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ssp.h"

static int num_reg;
static int num_dir;
static int num_cspec;
static int num_bspec;
static int num_fifo;
static int num_sock;
static int num_symlink;
static int num_door;

static int process (const char *path, const struct stat *stat_buf, int type,
	struct FTW *ftwp);

int main (int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++) {
		num_reg = num_dir = num_cspec = num_bspec = 0;
		num_fifo = num_sock = num_symlink = num_door = 0;

		nftw (argv [i], process, FOPEN_MAX, FTW_PHYS);

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

static int process (const char *path, const struct stat *stat_buf, int type,
	struct FTW *ftwp)
{
	switch (type) {
		case FTW_F:
			switch (stat_buf -> st_mode & S_IFMT) {
				case S_IFREG:
					num_reg++;
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

				case S_IFDOOR:
					num_door++;
					break;
			}
			break;

		case FTW_D:
			num_dir++;
			break;

		case FTW_SL:
		case FTW_SLN:
			num_symlink++;
			break;

		case FTW_DNR:
			err_ret ("Can't read directory: %s", path);
			break;

		case FTW_NS:
			err_ret ("Can't stat %s", path);
			break;
	}

	return (0);
}
