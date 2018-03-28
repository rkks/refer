#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

char     typeOfFile(mode_t);
char    *permOfFile(mode_t);
void     outputStatInfo(char *, char *, struct stat *);

int
main(int argc, char **argv)
{
    DIR *dp;
    char *dirname;
    struct stat st;
    struct dirent *d;
    char filename[BUFSIZ+1];

    /*
     * For each directory on the command line...
     */
    while (--argc) {
        dirname = *++argv;

        /*
         * Open the directory.
         */
        if ((dp = opendir(dirname)) == NULL) {
            perror(dirname);
            continue;
        }

        printf("%s:\n", dirname);

        /*
         * For each file in the directory...
         */
        while ((d = readdir(dp)) != NULL) {
            /*
             * Create the full file name.
             */
            sprintf(filename, "%s/%s", dirname, d->d_name);

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
            outputStatInfo(filename, d->d_name, &st);
            putchar('\n');
        }

        putchar('\n');
        closedir(dp);
    }

    exit(0);
}

/*
 * outputStatInfo - print out the contents of the stat structure.
 */
void
outputStatInfo(char *pathname, char *filename, struct stat *st)
{
    int n;
    struct group *gr;
    struct passwd *pw;
    char login[16], group[16], slink[BUFSIZ+1];

    /*
     * Print the number of file system blocks, permission bits,
     * and number of links.
     */
    printf("%5d ", st->st_blocks);
    printf("%c%s ", typeOfFile(st->st_mode), permOfFile(st->st_mode));
    printf("%3d ", st->st_nlink);

    /*
     * Look up the owner's login name.  Use the user-id if we
     * can't find it.
     */
    if ((pw = getpwuid(st->st_uid)) != NULL)
        strcpy(login, pw->pw_name);
    else
        sprintf(login, "%d", st->st_uid);
    
    /*
     * Look up the group's name.  Use the group-id if we
     * can't find it.
     */
    if ((gr = getgrgid(st->st_gid)) != NULL)
        strcpy(group, gr->gr_name);
    else
        sprintf(group, "%d", st->st_gid);

    /*
     * Print the owner and group.
     */
    printf("%-8s %-8s ", login, group);

    /*
     * If the file is not a device, print its size; otherwise
     * print its major and minor device numbers.
     */
    if (((st->st_mode & S_IFMT) != S_IFCHR) &&
        ((st->st_mode & S_IFMT) != S_IFBLK))
        printf("%9d ", st->st_size);
    else
        printf("%4d,%4d ", major(st->st_rdev), minor(st->st_rdev));

    /*
     * Print the access time.  The ctime() function is
     * described in Chapter 7, "Time and Timers".
     */
    printf("%.12s ", ctime(&st->st_mtime) + 4);

    /*
     * Print the file name.  If it's a symblic link, also print
     * what it points to.
     */
    printf("%s", filename);

    if ((st->st_mode & S_IFMT) == S_IFLNK) {
        if ((n = readlink(pathname, slink, sizeof(slink))) < 0)
            printf(" -> ???");
        else
            printf(" -> %.*s", n, slink);
    }
}

/*
 * typeOfFile - return the english description of the file type.
 */
char
typeOfFile(mode_t mode)
{
    switch (mode & S_IFMT) {
    case S_IFREG:
        return('-');
    case S_IFDIR:
        return('d');
    case S_IFCHR:
        return('c');
    case S_IFBLK:
        return('b');
    case S_IFLNK:
        return('l');
    case S_IFIFO:
        return('p');
    case S_IFSOCK:
        return('s');
    }

    return('?');
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
