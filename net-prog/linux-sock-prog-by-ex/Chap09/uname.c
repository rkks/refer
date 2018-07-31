/* uname.c:
 * 
 * Example of uname(2) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>

int
main(int argc,char **argv) {
    int z;
    struct utsname u_name;

    z = uname(&u_name);

    if ( z == -1 ) {
        fprintf(stderr,"%s: uname(2)\n",
            strerror(errno));
        exit(1);
    }

    printf("   sysname[] = '%s';\n",
        u_name.sysname);
    printf("  nodename[] = '%s';\n",
        u_name.nodename);
    printf("   release[] = '%s';\n",
        u_name.release);
    printf("   version[] = '%s';\n",
        u_name.version);
    printf("   machine[] = '%s';\n",
        u_name.machine);
    printf("domainname[] = '%s';\n",
        u_name.domainname);

    return 0;                    
}
