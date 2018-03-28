#include <sys/param.h>
#include <sys/time.h>
#include <sys/vnode.h>
#include <sys/inode.h>
#include <sys/ino.h>
#include <sys/fs.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>
#include <checklist.h>
#include <pwd.h>

#define sblock  sb_un.u_sblock

/*
 * We need a union to hold the super block, because it takes up an
 * entire disk block (the smallest unit in which you can read), but
 * the structure is not actually that big.
 */
union {
    struct fs u_sblock;
    char      u_dummy[SBSIZE];
} sb_un;

/*
 * Keep track of usage with this.  We need to save the uid so that
 * we can sort the array by number of blocks used.
 */
struct usage {
    int     u_uid;
    size_t  u_blocks;
} usageByUid[UID_MAX];

/*
 * Name of the file system defaults file.
 */
char    *checklistFile = "/etc/checklist";

int diskuse(char *);
int bread(int, daddr_t, char *, int);
int compare(const void *, const void *);

int
main(int argc, char **argv)
{
    int n;
    struct checklist *checklist;
    char *fsname;
    struct passwd *pwd;

    /*
     * For each file system...
     */
    while (--argc) {
        fsname = *++argv;

        /*
         * Look up the file system so we can get the
         * character device it's on.
         */
        if ((checklist = getfsfile(fsname)) == NULL) {
            fprintf(stderr, "%s: not found in %s.\n", fsname, checklistFile);
            continue;
        }
        
        /*
         * Zero out our counters.
         */
        memset(usageByUid, 0, UID_MAX * sizeof(struct usage));

        /*
         * Put the uids in the counters.  The array is
         * initially in uid order, but later we sort it
         * by blocks.
         */
        for (n = 0; n < UID_MAX; n++)
            usageByUid[n].u_uid = n;

        /*
         * Calculate disk usage.
         */
        if (diskuse(checklist->fs_spec) < 0)
            continue;

        /*
         * Sort the usage array by blocks.
         */
        qsort(usageByUid, UID_MAX, sizeof(struct usage), compare);

        /*
         * Print a header.
         */
        printf("%s (%s):\n", checklist->fs_dir, checklist->fs_spec);

        /*
         * Print the usage information.
         */
        for (n = 0; n < UID_MAX; n++) {
            /*
             * Skip users with no usage.
             */
            if (usageByUid[n].u_blocks == 0)
                continue;

            /*
             * Look up the login name.  If not found,
             * use the user-id.
             */
            if ((pwd = getpwuid(usageByUid[n].u_uid)) != NULL)
                printf("\t%-10s", pwd->pw_name);
            else
                printf("\t#%-9d", usageByUid[n].u_uid);

            /*
             * Print the usage.  The number we have is in
             * 512-byte (actually DEV_BSIZE) blocks; we
             * convert this to kbytes.
             */
            printf("\t%8d\n", usageByUid[n].u_blocks / 2);
        }

        putchar('\n');
    }

    exit(0);
}

/*
 * diskuse - tabulate disk usage for the named device.
 */
int
diskuse(char *device)
{
    ino_t ino;
    daddr_t iblk;
    int i, fd, nfiles;
    struct dinode itab[MAXBSIZE / sizeof(struct dinode)];

    /*
     * Open the device for reading.
     */
    if ((fd = open(device, O_RDONLY)) < 0) {
        perror(device);
        return(-1);
    }

    /*
     * Sync everything out to disk.
     */
    (void) sync();

    /*
     * Read in the superblock.
     */
    if (bread(fd, SBLOCK, (char *) &sblock, SBSIZE) < 0) {
        (void) close(fd);
        return(-1);
    }

    /*
     * The number of files (number of inodes) is equal to
     * the number of inodes per cylinder group times the
     * number of cylinder groups.
     */
    nfiles = sblock.fs_ipg * sblock.fs_ncg;

    for (ino = 0; ino < nfiles; ) {
        /*
         * Read in the inode table for this cylinder group.  The
         * fsbtodb macro converts a file system block number to
         * a disk block number.  The itod macro converts an inode
         * number to its file system block number.
         */
        iblk = fsbtodb(&sblock, itod(&sblock, ino));

        if (bread(fd, iblk, (char *) itab, sblock.fs_bsize) < 0) {
            (void) close(fd);
            return(-1);
        }

        /*
         * For each inode...
         */
        for (i = 0; i < INOPB(&sblock) && ino < nfiles; i++, ino++) {
            /*
             * Inodes 0 and 1 are not used.
             */
            if (ino < ROOTINO)
                continue;

            /*
             * Skip unallocated inodes.
             */
            if ((itab[i].di_mode & IFMT) == 0)
                continue;

            /*
             * Count the blocks as used.
             *
             * HP-UX does device i/o in units of BLKDEV_IOSIZE instead
             * of 512-byte blocks.
             */
            usageByUid[itab[i].di_uid].u_blocks += itab[i].di_blocks *
                (BLKDEV_IOSIZE / 512);
        }
    }

    return(0);
}

/*
 * bread - read count bytes into buf, starting at disk block blockno.
 */
int
bread(int fd, daddr_t blockno, char *buf, int count)
{
    /*
     * Seek to the right place.
     */
    if (lseek(fd, (long) blockno * DEV_BSIZE, SEEK_SET) < 0) {
        perror("lseek");
        return(-1);
    }

    /*
     * Read in the data.
     */
    if ((count = read(fd, buf, count)) < 0) {
        perror("read");
        return(-1);
    }

    return(count);
}

/*
 * compare - compare two usage structures for qsort.
 */
int
compare(const void *a, const void *b)
{
    struct usage *aa, *bb;

    aa = (struct usage *) a;
    bb = (struct usage *) b;
    
    return(bb->u_blocks - aa->u_blocks);
}
