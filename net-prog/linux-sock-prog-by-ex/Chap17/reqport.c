/* reqport.c
 *
 * Request a port from the sockserv :
 */
#include "common.h"

/*
 * Request a INADDR_ANY socket on the
 * port number requested:
 *
 * ARGUMENTS:
 *  s       Socket to send request on
 *  port    Port (host order) being requested
 *
 * RETURNS:
 *  >= 0    Socket to use
 *  -1      Failed: check errno
 */
int
reqport(int port) {
    int z;
    int s;                       /* socket */
    struct sockaddr_un a_srvr;/* serv. adr */

    /*
     * Create a Unix Socket:
     */
    s = socket(PF_LOCAL,SOCK_STREAM,0);
    if ( s == -1 )
        return -1;  /* Failed: check errno */

    /*
     * Create the abstract address of
     * the socket server :
     */
    memset(&a_srvr,0,sizeof a_srvr);
    a_srvr.sun_family = AF_LOCAL;
    strncpy(a_srvr.sun_path,
        "zSOCKET-SERVER",
        sizeof a_srvr.sun_path-1);
    a_srvr.sun_path[0] = 0;

    /*
     * Connect to the sock server:
     */
    z = connect(s,&a_srvr,sizeof a_srvr);
    if ( z == -1 )
        return -1;  /* Failed: check errno */

    /*
     * Now issue our request:
     */
    do  {
        z = write(s,&port,sizeof port);
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 )
        return -1;    /* Failed: see errno */

    /*
     * Now wait for a reply:
     */
    z = recv_fd(s);
    close(s);

    return z;             /* z == fd or -1 */
}
