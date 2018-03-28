#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mkdev.h>
#include <stdio.h>

char    *typeOfFile(mode_t);
char    *permOfFile(mode_t);
void     outputStatInfo(char *, struct stat *);

int
main(int argc, char **argv)
{
    char *filename;
    struct stat st;

    /*
     * For each file on the command line...
     */
    while (--argc) {
        filename = *++argv;

        /*
         * Find out about it.
         */
        if (lstat(filename, &st) < 0) {
            perror(filename);
            putchar('\n');
            continue;
        }

        /*
         * Print out the information.
         */
        outputStatInfo(filename, &st);
        putchar('\n');
    }

    exit(0);
}

/*
 * outputStatInfo - print out the contents of the stat structure.
 */
void
outputStatInfo(char *filename, struct stat *st)
{
    printf("File Name:          %s\n", filename);
    printf("File Type:          %s\n", typeOfFile(st->st_mode));

    /*
     * If the file is not a device, print its size and optimal
     * i/o unit; otherwise print its major and minor device
     * numbers.
     */
    if (((st->st_mode & S_IFMT) != S_IFCHR) &&
        ((st->st_mode & S_IFMT) != S_IFBLK)) {
        printf("File Size:          %d bytes, %d blocks\n", st->st_size,
               st->st_blocks);
        printf("Optimum I/O Unit:   %d bytes\n", st->st_blksize);
    }
    else {
        printf("Device Numbers:     Major: %u   Minor: %u\n",
               major(st->st_rdev), minor(st->st_rdev));
    }

    /*
     * Print the permission bits in both "ls" format and
     * octal.
     */
    printf("Permission Bits:    %s (%04o)\n", permOfFile(st->st_mode),
           st->st_mode & 07777);

    printf("Inode Number:       %u\n", st->st_ino);
    printf("Owner User-Id:      %d\n", st->st_uid);
    printf("Owner Group-Id:     %d\n", st->st_gid);
    printf("Link Count:         %d\n", st->st_nlink);

    /*
     * Print the major and minor device numbers of the
     * file system that contains the file.
     */
    printf("File System Device: Major: %u   Minor: %u\n",
           major(st->st_dev), minor(st->st_dev));

    /*
     * Print the access, modification, and change times.
     * The ctime() function converts the time to a human-
     * readable format; it is described in Chapter 7,
     * "Time of Day Operations."
     */
    printf("Last Access:        %s", ctime(&st->st_atime));
    printf("Last Modification:  %s", ctime(&st->st_mtime));
    printf("Last I-Node Change: %s", ctime(&st->st_ctime));
}

/*
 * typeOfFile - return the english description of the file type.
 */
char *
typeOfFile(mode_t mode)
{
    switch (mode & S_IFMT) {
    case S_IFREG:
        return("regular file");
    case S_IFDIR:
        return("directory");
    case S_IFCHR:
        return("character-special device");
    case S_IFBLK:
        return("block-special device");
    case S_IFLNK:
        return("symbolic link");
    case S_IFIFO:
        return("FIFO");
    case S_IFSOCK:
        return("UNIX-domain socket");
    }

    return("???");
}

/*
 * permOfFile - return the file permissions in an "ls"-like string.
 */
char *
permOfFile(mode_t mode)
{
    int i;
    char *p;
    static char perms[10];

    p = perms;
    strcpy(perms, "---------");

    /*
     * The permission bits are three sets of three
     * bits: user read/write/exec, group read/write/exec,
     * other read/write/exec.  We deal with each set
     * of three bits in one pass through the loop.
     */
    for (i=0; i < 3; i++) {
        if (mode & (S_IREAD >> i*3))
            *p = 'r';
        p++;

        if (mode & (S_IWRITE >> i*3))
            *p = 'w';
        p++;

        if (mode & (S_IEXEC >> i*3))
            *p = 'x';
        p++;
    }

    /*
     * Put special codes in for set-user-id, set-group-id,
     * and the sticky bit.  (This part is incomplete; "ls"
     * uses some other letters as well for cases such as
     * set-user-id bit without execute bit, and so forth.)
     */
    if ((mode & S_ISUID) != 0)
        perms[2] = 's';

    if ((mode & S_ISGID) != 0)
        perms[5] = 's';

    if ((mode & S_ISVTX) != 0)
        perms[8] = 't';

    return(perms);
}
