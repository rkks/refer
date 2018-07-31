/* wrapper.c:
 * 
 * Simple wrapper example:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"

#define LOGPATH "/tmp/wrapper.log"

int
main(int argc,char **argv,char **envp) {
    int z;
    struct sockaddr_in adr_clnt; /* Client */
    int alen;            /* Address length */
    char dgram[512];     /* Receive buffer */
    char *str_addr; /* String form of addr */

    /*
     * We must log denied attempts :
     */
    if ( log_open(LOGPATH) == -1 )
        exit(1);   /* Can't open log file! */

    log("wrapper started.\n");

    /*
     * Peek at datagram using MSG_PEEK :
     */
    alen = sizeof adr_clnt;      /* length */

    z = recvfrom(0, /* Socket on std input */
        dgram,         /* Receiving buffer */
        sizeof dgram,     /* Max recv size */
        MSG_PEEK,     /* Flags: Peek!!!!!! */
        (struct sockaddr *)&adr_clnt,
        &alen);      /* Addr len, in & out */

    if ( z < 0 )
        bail("recvfrom(2), peeking at client"
            " address.");
            
    /*
     * Convert IP address to string form:
     */
    str_addr = inet_ntoa(adr_clnt.sin_addr);

    if ( strcmp(str_addr,"127.7.7.7") != 0 ) {
        /*
         * Not our special 127.7.7.7 address:
         */
        log("Address %s port %d rejected.\n",
            str_addr, ntohs(adr_clnt.sin_port));

        /*
         * We must read this packet now without
         * the MSG_PEEK option to discard dgram:
         */
        z = recvfrom(0,          /* Socket */
            dgram,     /* Receiving buffer */
            sizeof dgram,  /* Max rcv size */
            0,               /* No flags!! */
            (struct sockaddr *)&adr_clnt,
            &alen);
    
        if ( z < 0 )
            bail("recvfrom(2), eating dgram");
        exit(1);
    }

    /*
     * Accept this dgram request, and 
     * launch the server:
     */
    log("Address %s port %d accepted.\n",
        str_addr, ntohs(adr_clnt.sin_port));

    /*
     * inetd has provided argv[0] from the
     * config file /etc/inetd.conf: we have
     * used this to indicate the server's
     * full pathname for this example. We
     * simply pass any other arguments and
     * environment as is.
     */
    log("Starting '%s'\n",argv[0]);
    log_close();    /* No longer need this */

    z = execve(argv[0],argv,envp);

    /*
     * If control returns, then execve(2)
     * failed for some reason:
     */
    log_open(LOGPATH);      /* Re-open log */
    bail("execve(2), starting server");
    return 1;
}
