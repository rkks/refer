/* bindacpt.c:
 *
 * socket, bind, listen & accept:
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
#include <netdb.h>
 
extern int
  mkaddr(void *addr,
  int *addrlen,
  char *str_addr,
  char *protocol);

/*
 * This function reports the error and
 * exits back to the shell :
 */
void
bail(const char *on_what) {
    if ( errno != 0 ) {
        fputs(strerror(errno),stderr);
        fputs(": ",stderr);
    }
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

/*
 * Call socket(2), bind(2),
 * listen(2) and accept(2) :
 *
 * Returns socket.
 */
int
BindAccept(char *addr) {
    int z;
    int s;
    struct sockaddr_in adr_srvr;
    struct sockaddr_in adr_clnt;
    int len_inet;

    /*
     * Create a TDP/IP socket to use :
     */
    s = socket(PF_INET,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Bind the server address:
     */
    len_inet = sizeof adr_srvr;
    z = mkaddr(&adr_srvr,&len_inet,
        addr,"tcp");

    if ( z != 0 ) {
        puts("Bad address/port");
        exit(1);
    }

    /*
     * Bind server address :
     */
    z = bind(s,(struct sockaddr *)&adr_srvr,
            len_inet);
    if ( z == -1 )
        bail("bind(2)");

    /*
     * Set listen mode :
     */
    if ( listen(s,10) == -1 )
        bail("listen(2)");

    /*
     * Wait for a connect :
     */
    len_inet = sizeof adr_clnt;

    z = accept(s,
        (struct sockaddr *)&adr_clnt,
        &len_inet);

    if ( z == -1 )
            bail("accept(2)");

    close(s);   /* No longer needed */
    return z;   /* Connected socket */
}

int
Connect(char *addr) {
    int z;
    int s;
    struct sockaddr_in adr_srvr;
    int len_inet;
    
    /*
     * Create a TDP/IP socket to use :
     */
    s = socket(PF_INET,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Bind the server address:
     */
    len_inet = sizeof adr_srvr;
    z = mkaddr(&adr_srvr,&len_inet,
        addr,"tcp");

    if ( z != 0 ) {
        puts("Bad address/port");
        exit(1);
    }

    /*
     * Connect to server :
     */
    len_inet = sizeof adr_srvr;

    z = connect(s,
        (struct sockaddr *)&adr_srvr,
        len_inet);

    if ( z == -1 )
            bail("connect(2)");

    return s;   /* Connected socket */
}

