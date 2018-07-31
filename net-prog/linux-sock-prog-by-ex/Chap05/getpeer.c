/* getpeer.c :
 *
 * Demonstrate getpeername(2) :
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
 * This function accepts as input a socket
 * for which a peer socket address must be
 * determined for it. Then the address
 * is converted into a string and returned.
 *
 * If an error occurs, NULL is returned.
 */
char *
peer_addr(int s,char *buf,size_t bufsiz) {
    int z;           /* Status return code */
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */

    /*
     * Obtain the address of the socket:
     */
    len_inet = sizeof adr_inet;

    z = getpeername(s,
        (struct sockaddr *)&adr_inet,
        &len_inet);

    if ( z == -1 )
        return NULL;    /* Failed */
     
    /*
     * Convert address into a string
     * form that can be displayed:
     */
    z = snprintf(buf,bufsiz,
        "%s:%u",
        inet_ntoa(adr_inet.sin_addr),
        (unsigned)ntohs(adr_inet.sin_port));

    if ( z == -1 )
        return NULL;    /* Buffer too small */

    return buf;
}
