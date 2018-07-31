/* inetaddr.c:
 * 
 * Example using inet_addr(3) :
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

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    int z;
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */
    int sck_inet;                /* Socket */

    /* Create a Socket */
    sck_inet = socket(AF_INET,SOCK_STREAM,0);

    if ( sck_inet == -1 )
        bail("socket()");

    /* Establish address */
    memset(&adr_inet,0,sizeof adr_inet);

    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(9000);

    adr_inet.sin_addr.s_addr =
        inet_addr("127.0.0.95");

    if ( adr_inet.sin_addr.s_addr == INADDR_NONE )
        bail("bad address.");

    len_inet = sizeof adr_inet;

    /* Bind it to the socket */
    z = bind(sck_inet,
        (struct sockaddr *)&adr_inet,
        len_inet);

    if ( z == -1 )
        bail("bind()");

    /* Display our socket address */
    system("netstat -pa --tcp 2>/dev/null"
        "| grep inetaddr");

    return 0;
}
