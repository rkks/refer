/* dgramisrvr.c:
 * 
 * Example inetd datagram server :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"

#define LOGPATH "/tmp/dgramisrvr.log"

int
main(int argc,char **argv) {
    int z;
    int s;                       /* Socket */
    int alen;         /* Length of address */
    struct sockaddr_in adr_clnt; /* Client */
    char dgram[512];     /* Receive buffer */
    char dtfmt[512];   /* Date/Time Result */
    time_t td;    /* Current Time and Date */
    struct tm dtv;     /* Date time values */
    fd_set rx_set;    /* Incoming req. set */
    struct timeval tmout; /* Timeout value */

    /*
     * Open a log file for append:
     */
    if ( log_open(LOGPATH) == -1 )
        exit(1);           /* No log file! */

    log("dgramisrvr started.\n");

    /*
     * Other initialization :
     */
    s = 0;   /* Our socket is on std input */
    FD_ZERO(&rx_set);        /* Initialize */
    FD_SET(s,&rx_set);      /* Notice fd=0 */

    /*
     * Now wait for incoming datagrams :
     */
    for (;;) {
        /*
         * Block until a datagram arrives:
         */
        alen = sizeof adr_clnt;

        z = recvfrom(s,          /* Socket */
            dgram,     /* Receiving buffer */
            sizeof dgram, /* Max recv size */
            0,        /* Flags: no options */
            (struct sockaddr *)&adr_clnt,
            &alen);  /* Addr len, in & out */

        if ( z < 0 )
            bail("recvfrom(2)");

        dgram[z] = 0; /* NUL terminate dgram */

        /*
         * Log the request :
         */
        log("Got request '%s' from %s port %d\n",
            dgram,
            inet_ntoa(adr_clnt.sin_addr),
            ntohs(adr_clnt.sin_port));

        /*
         * Get the current date and time:
         */
        time(&td);  /* current time & date */
        dtv = *localtime(&td);

        /*
         * Format a new date and time string,
         * based upon the input format string:
         */
        strftime(dtfmt, /* Formated result */
            sizeof dtfmt,      /* Max size */
            dgram,     /* date/time format */
            &dtv);         /* Input values */

        /*
         * Send the formatted result back to the
         * client program:
         */
        z = sendto(s,            /* Socket */
            dtfmt,      /* datagram result */
            strlen(dtfmt),       /* length */
            0,        /* Flags: no options */
            (struct sockaddr *)&adr_clnt,
            alen);

        if ( z < 0 )
            bail("sendto(2)");

        /*
         * Wait for next packet or timeout :
         *
         * This is easily accomplished with the
         * use of select(2).
         */
        do  {
            /* Establish Timeout = 8.0 secs */
            tmout.tv_sec = 8;   /* 8 seconds */
            tmout.tv_usec = 0;  /* + 0 usec */

            /* Wait for read event or timeout */
            z = select(s+1,&rx_set,NULL,NULL,&tmout);

        } while ( z == -1 && errno == EINTR );

        /*
         * Exit if select(2) returns an error
         * or if it indicates a timeout :
         */
        if ( z <= 0 )
            break;
    }

    /*
     * Close the socket and exit:
     */
    if ( z == -1 )
        log("%s: select(2)\n",strerror(errno));
    else
        log("Timed out: server exiting.\n");

    close(s);
    log_close();
    return 0;
}
