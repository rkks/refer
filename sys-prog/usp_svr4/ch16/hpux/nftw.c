#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <ftw.h>

int process(const char *, const struct stat *, int, struct FTW);

int
main(int argc, char **argv)
{
    while (--argc) {
        printf("Directory %s:\n", *++argv);

        nftw(*argv, process, sysconf(_SC_OPEN_MAX) - 3, 0);

        putchar('\n');
    }

    exit(0);
}

int
process(const char *path, const struct stat *st, int flag, struct FTW info)
{
    int i;

    for (i = 0; i < info.level; i++)
        printf("  ");

    printf("%-*s", 36 - 2 * info.level, &path[info.base]);

    switch (flag) {
    case FTW_F:
        printf("file, mode %o\n", st->st_mode & 07777);
        break;
    case FTW_D:
    case FTW_DP:
        printf("directory, mode %o\n", st->st_mode & 07777);
        break;
    case FTW_SL:
        printf("symbolic link to nowhere\n");
        break;
    case FTW_DNR:
        printf("unreadable directory, mode %o\n", st->st_mode & 07777);
        break;
    case FTW_NS:
        printf("unknown; stat() failed\n");
        break;
    }

    return(0);
}
