/* Listing 1.1 :
 *
 * Example of socketpair(2) function:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int
main(int argc,char **argv) {
    int z;                  /* Status return code */
    int s[2];               /* Pair of sockets */

    /*
     * Create a pair of local sockets :
     */
    z = socketpair(AF_LOCAL,SOCK_STREAM,0,s);

    if ( z == -1 ) {
        fprintf(stderr,
            "%s: socketpair(AF_LOCAL,SOCK_STREAM,0)\n",
            strerror(errno));
        return 1;           /* Failed */
    }

    /*
     * Report the socket file descriptors returned:
     */
    printf("s[0] = %d;\n",s[0]);
    printf("s[1] = %d;\n",s[1]);

    system("netstat --unix -p");

    return 0;
}
