#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <sys/mnttab.h>
#include <sys/vfstab.h>

char    *mnttabFile = "/etc/mnttab";
char    *vfstabFile = "/etc/vfstab";

struct statvfs  *getfsInfo(char *);
struct mnttab   *getmnttabEntry(FILE *);
struct vfstab   *getvfstabEntry(FILE *, struct mnttab *);

int
main(void)
{
    time_t clock;
    struct mnttab *mnt;
    struct vfstab *vfs;
    struct statvfs *stats;
    FILE *mnttabFP, *vfstabFP;

    /*
     * Open the mounted file system table.
     */
    if ((mnttabFP = fopen(mnttabFile, "r")) == NULL) {
        perror(mnttabFile);
        exit(1);
    }

    /*
     * Open the file system defaults file.
     */
    if ((vfstabFP = fopen(vfstabFile, "r")) == NULL) {
        perror(vfstabFile);
        exit(1);
    }

    /*
     * For each file system...
     */
    while ((mnt = getmnttabEntry(mnttabFP)) != NULL) {
        /*
         * If it's not an "ignore" file system, look it
         * up in the defaults file and get its current
         * stats.
         */
        if (hasmntopt(mnt, "ignore") == 0) {
            vfs = getvfstabEntry(vfstabFP, mnt);
            stats = getfsInfo(mnt->mnt_mountp);
        }
        else {
            stats = NULL;
            vfs = NULL;
        }

        clock = atoi(mnt->mnt_time);

        /*
         * Print the mnttab structure.
         */
        printf("%s:\n", mnt->mnt_mountp);
        printf("  %s information:\n", mnttabFile);
        printf("    file system type:     %s\n", mnt->mnt_fstype);
        printf("    mounted on device:    %s\n", mnt->mnt_special);
        printf("    mounted with options: %s\n", mnt->mnt_mntopts);
        printf("    mounted since:        %s", ctime(&clock));

        /*
         * Print the vfstab structure.
         */
        if (vfs != NULL) {
            printf("  %s information:\n", vfstabFile);
            printf("    file system type:     %s\n",
                   vfs->vfs_fstype ? vfs->vfs_fstype : "");
            printf("    mount device:         %s\n",
                   vfs->vfs_special ? vfs->vfs_special : "");
            printf("    fsck device:          %s\n",
                   vfs->vfs_fsckdev ? vfs->vfs_fsckdev : "");
            printf("    fsck pass number:     %s\n",
                   vfs->vfs_fsckpass ? vfs->vfs_fsckpass : "");
            printf("    mount at boot time:   %s\n",
                   vfs->vfs_automnt ? vfs->vfs_automnt : "");
            printf("    mount with options:   %s\n",
                   vfs->vfs_mntopts ? vfs->vfs_mntopts : "");
        }

        /*
         * Print the statvfs structure.
         */
        if (stats != NULL) {
            printf("  statvfs information:\n");
            printf("    maximum name length:  %u\n", stats->f_namemax);
            printf("    preferred block size: %u\n", stats->f_bsize);
            printf("    fundam. block size:   %u\n", stats->f_frsize);
            printf("    total blocks:         %u\n", stats->f_blocks);
            printf("    total blocks free:    %u\n", stats->f_bfree);
            printf("    total blocks avail:   %u\n", stats->f_bavail);
            printf("    total files:          %u\n", stats->f_files);
            printf("    total files free:     %u\n", stats->f_ffree);
            printf("    total files avail:    %u\n", stats->f_favail);
        }

        putchar('\n');
    }

    /*
     * All done.
     */
    fclose(mnttabFP);
    fclose(vfstabFP);
    exit(0);
}

/*
 * getmnttabEntry - read an entry from the mount table.
 */
struct mnttab *
getmnttabEntry(FILE *fp)
{
    int n;
    static int line = 0;
    static struct mnttab mnt;

    /*
     * Until we get a good entry...
     */
    for (;;) {
        /*
         * Read the next entry.
         */
        n = getmntent(fp, &mnt);
        line++;

        switch (n) {
        case 0:             /* okay             */
            return(&mnt);
        case -1:            /* end of file      */
            return(NULL);
        case MNT_TOOLONG:
            fprintf(stderr, "%s: %d: line too long.\n", mnttabFile, line);
            break;
        case MNT_TOOMANY:
            fprintf(stderr, "%s: %d: too many fields.\n", mnttabFile, line);
            break;
        case MNT_TOOFEW:
            fprintf(stderr, "%s: %d: not enough fields.\n", mnttabFile, line);
            break;
        }
    }
}

/*
 * getvfstabEntry - look up the file system defaults for the file system
 *          described by mnt.
 */
struct vfstab *
getvfstabEntry(FILE *fp, struct mnttab *mnt)
{
    struct vfstab vfsref;
    static struct vfstab vfs;

    /*
     * Have to rewind each time.
     */
    rewind(fp);

    /*
     * Zero out the reference structure.
     */
    memset((char *) &vfsref, 0, sizeof(struct vfstab));

    /*
     * Look for an entry that has the same special device,
     * mount point, and file system type.
     */
    vfsref.vfs_special = mnt->mnt_special;
    vfsref.vfs_mountp = mnt->mnt_mountp;
    vfsref.vfs_fstype = mnt->mnt_fstype;

    /*
     * Look it up.
     */
    if (getvfsany(fp, &vfs, &vfsref) == 0)
        return(&vfs);

    return(NULL);
}

/*
 * getfsInfo - look up information about the file system.
 */
struct statvfs *
getfsInfo(char *filsys)
{
    static struct statvfs stats;

    if (statvfs(filsys, &stats) < 0) {
        perror(filsys);
        return(NULL);
    }

    return(&stats);
}
