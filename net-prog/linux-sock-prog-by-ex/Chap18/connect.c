/* Connect.c
 *
 * Connect to a remote host:
 */
#include "quotes.h"

/*
 * ARGUMENTS:
 *  addr    String address in mkaddr()
 *          format.
 *
 * RETURNS:
 *  0       Successful
 *  -1      Failed
 */
int
Connect(const char *addr) {
    int z;                         /* Status */
    int s;                         /* Socket */
    int sa_len;                /* Addr. len. */
    struct sockaddr_in sa_srvr; /* srvr addr */

    /*
     * Establish connect address:
     */
    sa_len = sizeof sa_srvr;
    z = mkaddr(&sa_srvr,&sa_len,
        (char *)addr,"tcp");
    if ( z != 0 ) {
        msgf('e',
            "Bad address: connect(%s)",
            addr);
        return -1;
    }

    /*
     * Create a socket and connect:
     */
    s = socket(PF_INET,SOCK_STREAM,0);

    do  {
        z = connect(s,&sa_srvr,sa_len);
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 ) {
        msgf('e',
            "%s: connect(%s)",
            strerror(errno),
            addr);
        return -1;
    }

    return s;     /* Return connected socket */
}
