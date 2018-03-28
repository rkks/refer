#include <sys/utsname.h>
#include <stdio.h>

int
main(void)
{
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
    printf("HP-UX 10.x does not provide the sysinfo() function.  Therefore,\n");
    printf("this part of the example does not do anything.\n");

    exit(0);
}
