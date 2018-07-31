/* dgramcln2.c:
 * 
 * Modified datagram client :
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

    if ( errno ) {
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
    char *srvr_addr = NULL;   /* Srvr addr */
    char *clnt_addr = NULL;   /* Clnt addr */
    struct sockaddr_in adr_srvr; /* Server */
    struct sockaddr_in adr_clnt; /* Client */
    struct sockaddr_in adr;     /* AF_INET */
    int alen;        /* Socket addr length */
    int s;                       /* Socket */
    char dgram[512];        /* Recv buffer */

    /*
     * Insist on two command line arguments
     * (without port numbers):
     *
     * dgramcln2 <server_addr> <client_addr>
     */
    if ( argc != 3 ) {
        fputs("Usage: dgramclnt <server_ipaddr> "
            "<client_ipaddr>\n",stderr);
        return 1;
    }

    srvr_addr = argv[1]; /* 1st arg is srv */
    clnt_addr = argv[2]; /* 2nd arg is cln */

    /*
     * Create a server socket address:
     */
    memset(&adr_srvr,0,sizeof adr_srvr);
    adr_srvr.sin_family = AF_INET;
    adr_srvr.sin_port = htons(9090);
    adr_srvr.sin_addr.s_addr =
        inet_addr(srvr_addr);

    if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
        bail("bad server address.");

    /*
     * Create a UDP socket:
     */
    s = socket(AF_INET,SOCK_DGRAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Create the specific client address:
     */
    memset(&adr_clnt,0,sizeof adr_clnt);
    adr_clnt.sin_family = AF_INET;
    adr_clnt.sin_port = 0;     /* Any port */
    adr_clnt.sin_addr.s_addr =
        inet_addr(clnt_addr);

    if ( adr_clnt.sin_addr.s_addr == INADDR_NONE )
        bail("bad client address.");

    /*
     * Bind the specific client address :
     */
    z = bind(s, (struct sockaddr *)&adr_clnt,
        sizeof adr_clnt);

    if ( z == -1 )
        bail("bind(2) of client address");

    /*
     * Enter input client loop :
     */
    for (;;) {
        /*
         * Prompt user for a date format string:
         */
        fputs("\nEnter format string: ",stdout);
        if ( !fgets(dgram,sizeof dgram,stdin) )
            break;                  /* EOF */

        z = strlen(dgram);
        if ( z > 0 && dgram[--z] == '\n' )
            dgram[z] = 0;  /* Stomp out newline */

        /*
         * Send format string to server:
         */
        z = sendto(s,            /* Socket */
            dgram,      /* datagram to snd */
            strlen(dgram), /* dgram length */
            0,        /* Flags: no options */
            (struct sockaddr *)&adr_srvr,
            sizeof adr_srvr);

        if ( z < 0 )
            bail("sendto(2)");

        /*
         * Wait for a response :
         *
         * NOTE: Control will hang here if the
         * wrapper decides we lack access (no
         * response will arrive.)
         */
        alen = sizeof adr;

        z = recvfrom(s,          /* Socket */
            dgram,     /* Receiving buffer */
            sizeof dgram, /* Max recv size */
            0,        /* Flags: no options */
            (struct sockaddr *)&adr,
            &alen);  /* Addr len, in & out */

        if ( z < 0 )
            bail("recvfrom(2)");

        dgram[z] = 0;     /* NUL terminate */

        /*
         * Report Result :
         */
        printf("Result from %s port %u :"
            "\n\t'%s'\n",
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
