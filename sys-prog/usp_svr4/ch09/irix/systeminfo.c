#include <sys/systeminfo.h>
#include <sys/utsname.h>
#include <stdio.h>

typedef struct {
    int     command;
    char    *string;
} Info;

Info info[] = {
    SI_SYSNAME,         "SI_SYSNAME",
    SI_HOSTNAME,        "SI_HOSTNAME",
    SI_RELEASE,         "SI_RELEASE",
    SI_VERSION,         "SI_VERSION",
    SI_MACHINE,         "SI_MACHINE",
    SI_ARCHITECTURE,    "SI_ARCHITECTURE",
    SI_HW_PROVIDER,     "SI_HW_PROVIDER",
    SI_HW_SERIAL,       "SI_HW_SERIAL",
    SI_SRPC_DOMAIN,     "SI_SRPC_DOMAIN",
    0,                  NULL
};

int
main(void)
{
    Info *ip;
    char buf[BUFSIZ];
    struct utsname name;

    /*
     * Request uname information.
     */
    if (uname(&name) < 0) {
        perror("uname");
        exit(1);
    }

    /*
     * Print it out.
     */
    printf("Uname information:\n");
    printf("\t sysname: %s\n", name.sysname);
    printf("\tnodename: %s\n", name.nodename);
    printf("\t release: %s\n", name.release);
    printf("\t version: %s\n", name.version);
    printf("\t machine: %s\n", name.machine);

    /*
     * Request and print system information.
     */
    printf("\nSysinfo information:\n");

    for (ip = info; ip->string != NULL; ip++) {
        if (sysinfo(ip->command, buf, sizeof(buf)) < 0) {
            perror("sysinfo");
            exit(1);
        }

        printf("%16s: %s\n", ip->string, buf);
    }

    exit(0);
}
