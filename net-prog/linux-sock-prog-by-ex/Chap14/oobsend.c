/* oobsend.c:
 * 
 * Example OOB sender :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

extern void bail(char *on_what);
extern int Connect(char *addr);

/*
 * Send in-band data :
 */
static void
iband(int s,char *str) {
    int z;

    z = send(s,str,strlen(str),0);
    if ( z == -1 )
        bail("send(2)");

    printf("ib: '%s' (%d)\n",str,z);
}

/*
 * Send out-of-band data :
 */
static void
oband(int s,char *str) {
    int z;

    z = send(s,str,strlen(str),MSG_OOB);
    if ( z == -1 )
        bail("send(2)");

    printf("OOB '%s' (%d)\n",str,z);
}

int
main(int argc,char **argv) {
    int s = -1;     /* Socket */

    s = Connect(argc >= 2
        ? argv[1]
        : "127.0.0.1:9011");

    iband(s,"In the beginning");
    sleep(1);

    iband(s,"Linus begat Linux,");
    sleep(1);

    iband(s,"and the Penguins");
    sleep(1);

    oband(s,"rejoiced");
    sleep(1);

    iband(s,"exceedingly.");
    close(s);

    return 0;
}
