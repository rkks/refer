/* inetntoa.c:
 * 
 * Example using inet_ntoa(3) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int
main(int argc,char **argv) {
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */

    /*
     * Establish address (pretend we got
     * this address from a connecting
     * client) :
     */
    memset(&adr_inet,0,sizeof adr_inet);

    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(9000);

    if ( !inet_aton("127.0.0.23",
              &adr_inet.sin_addr) )
        puts("bad address.");

    len_inet = sizeof adr_inet;

    /*
     * Demonstrate use of inet_ntoa(3) :
     */
    printf("The IP Address is %s\n",
        inet_ntoa(adr_inet.sin_addr));

    return 0;
}
