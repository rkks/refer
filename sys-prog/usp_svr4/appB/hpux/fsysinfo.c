#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <checklist.h>
#include <mntent.h>

char    *mnttabFile = "/etc/mnttab";
char    *vfstabFile = "/etc/fstab";

struct statvfs  *getfsInfo(char *);
struct mntent   *getmnttabEntry(FILE *);
struct mntent   *getvfstabEntry(FILE *, struct mntent *);

int
main(void)
{
    time_t clock;
    struct mntent *mnt;
    struct mntent *vfs;
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
            stats = getfsInfo(mnt->mnt_dir);
        }
        else {
            stats = NULL;
            vfs = NULL;
        }

        clock = mnt->mnt_time;

        /*
         * Print the mnttab structure.
         */
        printf("%s:\n", mnt->mnt_dir);
        printf("  %s information:\n", mnttabFile);
        printf("    file system type:     %s\n", mnt->mnt_type);
        printf("    mounted on device:    %s\n", mnt->mnt_fsname);
        printf("    mounted with options: %s\n", mnt->mnt_opts);
        printf("    mounted since:        %s", ctime(&clock));

        /*
         * Print the vfstab structure.
         */
        if (vfs != NULL) {
            printf("  %s information:\n", vfstabFile);
            printf("    file system type:     %s\n",
                   vfs->mnt_type ? vfs->mnt_type : "");
            printf("    mount device:         %s\n",
                   vfs->mnt_fsname ? vfs->mnt_fsname : "");
            printf("    dump frequency:       %d\n", vfs->mnt_freq);
            printf("    fsck pass number:     %d\n", vfs->mnt_passno);
            printf("    mount with options:   %s\n",
                   vfs->mnt_opts ? vfs->mnt_opts : "");
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
struct mntent *
getmnttabEntry(FILE *fp)
{
    struct mntent *mnt;
    static struct mntent mntsave;

    if ((mnt = getmntent(fp)) == NULL)
        return(NULL);

    mntsave.mnt_fsname = strdup(mnt->mnt_fsname);
    mntsave.mnt_dir = strdup(mnt->mnt_dir);
    mntsave.mnt_type = strdup(mnt->mnt_type);
    mntsave.mnt_opts = strdup(mnt->mnt_opts);
    mntsave.mnt_freq = mnt->mnt_freq;
    mntsave.mnt_passno = mnt->mnt_passno;
    mntsave.mnt_time = mnt->mnt_time;
    mntsave.mnt_cnode = mnt->mnt_cnode;

    return(&mntsave);
}

/*
 * getvfstabEntry - look up the file system defaults for the file system
 *          described by mnt.
 */
struct mntent *
getvfstabEntry(FILE *fp, struct mntent *mnt)
{
    static struct mntent mntref;

    /*
     * Have to rewind each time.
     */
    rewind(fp);

    /*
     * Look for an entry that has the same special device,
     * mount point, and file system type.
     */
    mntref.mnt_fsname = strdup(mnt->mnt_fsname);
    mntref.mnt_type = strdup(mnt->mnt_type);
    mntref.mnt_dir = strdup(mnt->mnt_dir);

    /*
     * Look it up.
     */
    while ((mnt = getmntent(fp)) != NULL) {
        if (strcmp(mnt->mnt_fsname, mntref.mnt_fsname) == 0 &&
            strcmp(mnt->mnt_type, mntref.mnt_type) == 0 &&
            strcmp(mnt->mnt_dir, mntref.mnt_dir) == 0)
            return(mnt);
    }

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
