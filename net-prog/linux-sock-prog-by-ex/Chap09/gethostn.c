/* gethostn.c:
 * 
 * Example of gethostname(2) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int
main(int argc,char **argv) {
    int z;
    char buf[32];

    z = gethostname(buf,sizeof buf);

    if ( z == -1 ) {
        fprintf(stderr,"%s: gethostname(2)\n",
            strerror(errno));
        exit(1);
    }

    printf("host name = '%s'\n",buf);

    z = getdomainname(buf,sizeof buf);
    
    if ( z == -1 ) {
        fprintf(stderr,"%s: getdomainname(2)\n",
            strerror(errno));
        exit(1);
    }

    printf("domain name = '%s'\n",buf);

    return 0;
}
