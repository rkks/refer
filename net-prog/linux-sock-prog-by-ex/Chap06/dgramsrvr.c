/* dgramsrvr.c:
 * 
 * Example datagram server :
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
    char *srvr_addr = NULL;
    struct sockaddr_in adr_inet;/* AF_INET */
    struct sockaddr_in adr_clnt;/* AF_INET */
    int len_inet;               /* length  */
    int s;                       /* Socket */
    char dgram[512];        /* Recv buffer */
    char dtfmt[512];   /* Date/Time Result */
    time_t td;    /* Current Time and Date */
    struct tm tm;      /* Date time values */

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
     * Create a UDP socket to use :
     */
    s = socket(AF_INET,SOCK_DGRAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Create a socket address, for use
     * with bind(2) :
     */
    memset(&adr_inet,0,sizeof adr_inet);
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(9090);
    adr_inet.sin_addr.s_addr =
        inet_addr(srvr_addr);

    if ( adr_inet.sin_addr.s_addr == INADDR_NONE )
        bail("bad address.");

    len_inet = sizeof adr_inet;

    /*
     * Bind a address to our socket, so that
     * client programs can contact this
     * server:
     */
    z = bind(s,
        (struct sockaddr *)&adr_inet,
        len_inet);
    if ( z == -1 )
        bail("bind()");

    /*
     * Now wait for requests:
     */
    for (;;) {
        /*
         * Block until the program receives a
         * datagram at our address and port:
         */
        len_inet = sizeof adr_clnt;
        z = recvfrom(s,                /* Socket */
            dgram,           /* Receiving buffer */
            sizeof dgram,   /* Max recv buf size */
            0,              /* Flags: no options */
            (struct sockaddr *)&adr_clnt,/* Addr */
            &len_inet);    /* Addr len, in & out */
        if ( z < 0 )
            bail("recvfrom(2)");

        /*
         * Process the request :
         */
        dgram[z] = 0;          /* null terminate */
        if ( !strcasecmp(dgram,"QUIT") )
            break;                /* Quit server */

        /*
         * Get the current date and time:
         */
        time(&td);    /* Get current time & date */
        tm = *localtime(&td);  /* Get components */

        /*
         * Format a new date and time string,
         * based upon the input format string:
         */
        strftime(dtfmt,      /* Formatted result */
            sizeof dtfmt,     /* Max result size */
            dgram,     /* Input date/time format */
            &tm);      /* Input date/time values */

        /*
         * Send the formatted result back to the
         * client program:
         */
        z = sendto(s,   /* Socket to send result */
            dtfmt, /* The datagram result to snd */
            strlen(dtfmt), /* The datagram lngth */
            0,              /* Flags: no options */
            (struct sockaddr *)&adr_clnt,/* addr */
            len_inet);  /* Client address length */
        if ( z < 0 )
            bail("sendto(2)");
    }

    /*
     * Close the socket and exit:
     */
    close(s);
    return 0;
}
