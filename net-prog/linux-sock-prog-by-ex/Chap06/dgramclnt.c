/* dgramclnt.c:
 * 
 * Example datagram client :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    char *srvr_addr = NULL;
    struct sockaddr_in adr_srvr;/* AF_INET */
    struct sockaddr_in adr;     /* AF_INET */
    int len_inet;               /* length  */
    int s;                       /* Socket */
    char dgram[512];        /* Recv buffer */

    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.23 :
     */
    if ( argc >= 2 ) {
        /* Addr on cmdline: */
        srvr_addr = argv[1];
    } else {
        /* Use default address: */
        srvr_addr = "127.0.0.23";
    }

    /*
     * Create a socket address, to use
     * to contact the server with:
     */
    memset(&adr_srvr,0,sizeof adr_srvr);
    adr_srvr.sin_family = AF_INET;
    adr_srvr.sin_port = htons(9090);
    adr_srvr.sin_addr.s_addr =
        inet_addr(srvr_addr);

    if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
        bail("bad address.");

    len_inet = sizeof adr_srvr;

    /*
     * Create a UDP socket to use :
     */
    s = socket(AF_INET,SOCK_DGRAM,0);
    if ( s == -1 )
        bail("socket()");

    for (;;) {
        /*
         * Prompt user for a date format string:
         */
        fputs("\nEnter format string: ",stdout);
        if ( !fgets(dgram,sizeof dgram,stdin) )
            break;                        /* EOF */

        z = strlen(dgram);
        if ( z > 0 && dgram[--z] == '\n' )
            dgram[z] = 0;   /* Stomp out newline */

        /*
         * Send format string to server:
         */
        z = sendto(s,   /* Socket to send result */
            dgram, /* The datagram result to snd */
            strlen(dgram), /* The datagram lngth */
            0,              /* Flags: no options */
            (struct sockaddr *)&adr_srvr,/* addr */
            len_inet);  /* Server address length */
        if ( z < 0 )
            bail("sendto(2)");

        /*
         * Test if we asked for a server shutdown:
         */
        if ( !strcasecmp(dgram,"QUIT") )
            break;          /* Yes, we quit too */

        /*
         * Wait for a response :
         */
        x = sizeof adr;
        z = recvfrom(s,                /* Socket */
            dgram,           /* Receiving buffer */
            sizeof dgram,   /* Max recv buf size */
            0,              /* Flags: no options */
            (struct sockaddr *)&adr,     /* Addr */
            &x);           /* Addr len, in & out */
        if ( z < 0 )
            bail("recvfrom(2)");

        dgram[z] = 0;          /* null terminate */

        /*
         * Report Result :
         */
        printf("Result from %s port %u :\n\t'%s'\n",
            inet_ntoa(adr.sin_addr),
            (unsigned)ntohs(adr.sin_port),
            dgram);
    }

    /*
     * Close the socket and exit:
     */
    close(s);
    putchar('\n');

    return 0;
}
