#include <sys/types.h>
#include <sys/time.h>
#include <lastlog.h>
#include <stdio.h>
#include <pwd.h>

int
main(int argc, char **argv)
{
    FILE *fp;
    struct lastlog ll;
    struct passwd *pwd;

    /*
     * Open the lastlog file.
     */
    if ((fp = fopen("/var/adm/lastlog", "r")) == NULL) {
        perror("/var/adm/lastlog");
        exit(1);
    }

    /*
     * For each user named on the command line...
     */
    while (--argc) {
        /*
         * Look up the user's user-id number.
         */
        if ((pwd = getpwnam(*++argv)) == NULL) {
            fprintf(stderr, "unknown user: %s\n", *argv);
            continue;
        }

        /*
         * Read the right structure.
         */
        fseek(fp, pwd->pw_uid * sizeof(struct lastlog), 0);
        fread(&ll, sizeof(struct lastlog), 1, fp);

        /*
         * Print it out.
         */
        printf("%-8.8s %-8.8s %-16.16s %s", *argv, ll.ll_line, ll.ll_host,
               ctime(&ll.ll_time));
    }

    fclose(fp);
    exit(0);
}
