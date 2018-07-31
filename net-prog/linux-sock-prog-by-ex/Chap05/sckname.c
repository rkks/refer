/* sckname.c :
 *
 * Demonstrate getsockname(2) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * This saves lines of code later:
 */
static void
bail(const char *on_what) {
    perror(on_what);    /* Report error */
    exit(1);            /* Exit Program */
}

/*
 * This function accepts as input a socket
 * for which a socket address must be
 * determined for it. Then the address
 * is converted into a string and returned.
 *
 * If an error occurs, NULL is returned.
 */
char *
sock_addr(int s,char *buf,size_t bufsiz) {
    int z;           /* Status return code */
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */

    /*
     * Obtain the address of the socket:
     */
    len_inet = sizeof adr_inet;

    z = getsockname(s,
        (struct sockaddr *)&adr_inet,
        &len_inet);

    if ( z == -1 )
        return NULL;    /* Failed */
     
    /*
     * Convert address into a string
     * form that can be displayed:
     */
    snprintf(buf,bufsiz,
        "%s:%u",
        inet_ntoa(adr_inet.sin_addr),
        (unsigned)ntohs(adr_inet.sin_port));

    return buf;
}

/*
 * Main Program :
 */
int
main(int argc,char **argv,char **envp) {
    int z;           /* Status return code */
    int sck_inet;               /* Socket  */
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */
    char buf[64];           /* Work buffer */

    /*
     * Create an IPv4 Internet Socket:
     */
    sck_inet = socket(AF_INET,SOCK_STREAM,0);

    if ( sck_inet == -1 )
        bail("socket()");

    /*
     * Create an AF_INET address:
     */
    memset(&adr_inet,0,sizeof adr_inet);
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(9000);
    inet_aton("127.0.0.24",&adr_inet.sin_addr);
    len_inet = sizeof adr_inet;

    /*
     * Now bind the address to the socket:
     */
    z = bind(sck_inet,
        (struct sockaddr *)&adr_inet,
        len_inet);
    if ( z == -1 )
        bail("bind()");

    /*
     * Now test our sock_addr() function :
     */
    if ( !sock_addr(sck_inet,buf,sizeof buf) )
        bail("sock_addr()");

    printf("Address is '%s'\n",buf);

    close(sck_inet);
    return 0;
}

