/* gquotes.c:
 * 
 * Get datagram stock market
 * quotes from UDP broadcast :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

extern int mkaddr(
    void *addr,
    int *addrlen,
    char *str_addr,
    char *protocol);

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    int z;
    int x;
    struct sockaddr_in adr;     /* AF_INET */
    int len_inet;               /* length  */
    int s;                       /* Socket */
    char dgram[512];        /* Recv buffer */
    static int so_reuseaddr = TRUE;
    static char
        *bc_addr = "127.255.255.255:9097";

    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.23 :
     */
    if ( argc > 1 )
        /* Broadcast address : */
        bc_addr = argv[1];

    /*
     * Create a UDP socket to use :
     */
    s = socket(AF_INET,SOCK_DGRAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Form the broadcast address :
     */
    len_inet = sizeof adr;

    z = mkaddr(&adr,
        &len_inet,
        bc_addr,
        "udp");

    if ( z == -1 )
        bail("Bad broadcast address");

    /*
     * Allow multiple listeners on the
     * broadcast address :
     */
    z = setsockopt(s,
        SOL_SOCKET,
        SO_REUSEADDR,
        &so_reuseaddr,
        sizeof so_reuseaddr);
    
    if ( z == -1 )
        bail("setsockopt(SO_REUSEADDR)");

    /*
     * Bind our socket to the broadcast address:
     */
    z = bind(s,
        (struct sockaddr *)&adr,
        len_inet);

    if ( z == -1 )
        bail("bind(2)");

    for (;;) {
        /*
         * Wait for a broadcast message :
         */
        z = recvfrom(s,            /* Socket */
            dgram,       /* Receiving buffer */
            sizeof dgram,/* Max rcv buf size */
            0,          /* Flags: no options */
            (struct sockaddr *)&adr, /* Addr */
            &x);       /* Addr len, in & out */

        if ( z < 0 )
            bail("recvfrom(2)"); /* else err */

        fwrite(dgram,z,1,stdout);
        putchar('\n');

        fflush(stdout);
    }

    return 0;
}
