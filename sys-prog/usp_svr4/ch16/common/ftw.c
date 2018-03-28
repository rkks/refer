#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <ftw.h>

int process(const char *, const struct stat *, int);

int
main(int argc, char **argv)
{
    while (--argc) {
        printf("Directory %s:\n", *++argv);

        ftw(*argv, process, sysconf(_SC_OPEN_MAX) - 3);

        putchar('\n');
    }

    exit(0);
}

int
process(const char *path, const struct stat *st, int flag)
{
    printf("%-24s", path);

    switch (flag) {
    case FTW_F:
        printf("file, mode %o\n", st->st_mode & 07777);
        break;
    case FTW_D:
        printf("directory, mode %o\n", st->st_mode & 07777);
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
