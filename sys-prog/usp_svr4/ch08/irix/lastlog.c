#include <sys/types.h>
#include <sys/time.h>
#include <lastlog.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    FILE *fp;
    struct lastlog ll;
    char lastlogfile[1024];

    /*
     * For each user named on the command line...
     */
    while (--argc) {
        /*
         * Open the lastlog file.
         */
        sprintf(lastlogfile, "/var/adm/lastlog/%s", *++argv);

        if ((fp = fopen(lastlogfile, "r")) == NULL) {
            perror(lastlogfile);
            continue;
        }

        /*
         * Read the structure.
         */
        fread(&ll, sizeof(struct lastlog), 1, fp);

        /*
         * Print it out.
         */
        printf("%-8.8s %-8.8s %-16.16s %s", *argv, ll.ll_line, ll.ll_host,
               ctime(&ll.ll_time));

        fclose(fp);
    }

    exit(0);
}
