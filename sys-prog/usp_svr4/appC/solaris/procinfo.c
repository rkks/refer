#include <sys/param.h>
#include <sys/signal.h>
#include <sys/fault.h>
#include <sys/syscall.h>
#include <sys/procfs.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>

char    *procFileSystem = "/proc";

void    printTime(char *, time_t);
void    printProcInfo(prpsinfo_t *, prusage_t *);

int
main(int argc, char **argv)
{
    int fd;
    prusage_t prusage;
    prpsinfo_t prpsinfo;
    char procname[BUFSIZ], tmp[BUFSIZ];

    /*
     * For each argument...
     */
    while (--argc) {
        /*
         * Create the file name in the proc file system.
         */
        sprintf(procname, "%s/%s", procFileSystem, *++argv);

        /*
         * Open the file.
         */
        if ((fd = open(procname, O_RDONLY)) < 0) {
            perror(procname);
            continue;
        }

        /*
         * Get the "ps" information.
         */
        if (ioctl(fd, PIOCPSINFO, &prpsinfo) < 0) {
            sprintf(tmp, "%s: PIOCPSINFO", procname);
            perror(tmp);
            close(fd);
            continue;
        }

        /*
         * Get the resource usage information.
         */
        if (ioctl(fd, PIOCUSAGE, &prusage) < 0) {
            sprintf(tmp, "%s: PIOCPRUSAGE", procname);
            perror(tmp);
            close(fd);
            continue;
        }

        /*
         * Print the information.
         */
        printProcInfo(&prpsinfo, &prusage);
        close(fd);
    }

    exit(0);
}

/*
 * printProcInfo - print "interesting" fields of the prpsinfo and prusage
 *         structures.
 */
void
printProcInfo(prpsinfo_t *prpsinfo, prusage_t *prusage)
{
    printf("Command: %s\n", prpsinfo->pr_psargs);
    printf("Started at: %s", ctime(&prpsinfo->pr_start.tv_sec));
    printf("Process-ID: %d  Parent Process-ID: %d\n", prpsinfo->pr_pid,
           prpsinfo->pr_ppid);
    printf("Process Group Leader: %d  Session-ID: %d\n", prpsinfo->pr_pgrp,
           prpsinfo->pr_sid);
    printf("User-ID: %d  Group-ID: %d  ", prpsinfo->pr_uid,
           prpsinfo->pr_gid);
    printf("Priority: %d  Nice: %d\n", prpsinfo->pr_pri, prpsinfo->pr_nice);
    printf("Process Size: %d KB  Resident Set Size: %d KB\n",
           prpsinfo->pr_bysize / 1024, prpsinfo->pr_byrssize / 1024);
    printTime("Process Elapsed Time", prusage->pr_rtime.tv_sec);
    printTime("  Process User CPU Time", prusage->pr_utime.tv_sec);
    putchar('\n');
    printTime("Process System Call Time", prusage->pr_stime.tv_sec);
    printTime("  Process System Trap Time", prusage->pr_ttime.tv_sec);
    putchar('\n');
    printTime("Process Page Fault Time", prusage->pr_tftime.tv_sec +
          prusage->pr_dftime.tv_sec + prusage->pr_kftime.tv_sec);
    printTime("  Process Sleep Time", prusage->pr_ltime.tv_sec +
          prusage->pr_slptime.tv_sec + prusage->pr_wtime.tv_sec);
    putchar('\n');
    printTime("Process Stopped Time", prusage->pr_stoptime.tv_sec);
    putchar('\n');
    printf("Major Page Faults: %d  Minor Page Faults: %d  Swaps: %d\n",
           prusage->pr_majf, prusage->pr_minf, prusage->pr_nswap);
    printf("Input Blocks: %d  Output Blocks: %d  Character I/O: %d\n",
           prusage->pr_inblk, prusage->pr_oublk, prusage->pr_ioch);
    printf("System Calls: %d  Signals Received: %d\n", prusage->pr_sysc,
           prusage->pr_sigs);
    putchar('\n');
}

/*
 * printTime - convert a number of seconds to days, hours, minutes, and
 *         seconds, and print it out.
 */
void
printTime(char *str, time_t secs)
{
    int d, h, m, s;

    s = secs;

    /*
     * Simple conversion to days, hours, minutes, seconds.
     */
    d = s / 86400;
    s = s % 86400;
    h = s / 3600;
    s = s % 3600;
    m = s / 60;
    s = s % 60;

    /*
     * Print the label.
     */
    printf("%s: ", str);

    /*
     * Print the days.
     */
    if (d)
        printf("%dd", d);

    /*
     * Print the hours, minutes, and seconds.
     */
    printf("%02d:%02d:%02d", h, m, s);
}
