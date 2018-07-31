/* inetdserv.c:
 * 
 * Example inetd daytime server :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    if ( errno != 0 ) {
        fputs(strerror(errno),stderr);
        fputs(": ",stderr);
    }
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    int z;
    int n;
    time_t td;        /* Current date&time */
    char dtbuf[128];     /* Date/Time info */

    /*
     * Generate a time stamp :
     */
    time(&td);
    n = (int) strftime(dtbuf,sizeof dtbuf,
        "%A %b %d %H:%M:%S %Y\n",
         localtime(&td));

    /*
     * Write result back to the client :
     */
    z = write(1,dtbuf,n);
    if ( z == -1 )
        bail("write(2)");

    return 0;
}
