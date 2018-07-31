/* rpnsrv.c:
 * 
 * Example RPN Server :
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

#ifndef SHUT_RDWR
#define SHUT_RDWR 3
#endif

extern int mkaddr(void *addr,
    int *addr_len,
    char *input_address,
    char *protocol);

extern void rpn_process(FILE *tx,
    char *buf);


/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    if ( errno != 0 ) {
        fputs(strerror(errno),stderr);
        fputs(": ",stderr);
    }
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    int z;
    char *srvr_addr = "127.0.0.1:9090";
    struct sockaddr_in adr_srvr;/* AF_INET */
    struct sockaddr_in adr_clnt;/* AF_INET */
    int len_inet;               /* length  */
    int s = -1;                  /* Socket */
    int c = -1;           /* Client socket */
    FILE *rx = NULL;        /* Read stream */
    FILE *tx = NULL;       /* Write stream */
    char buf[4096];          /* I/O Buffer */

    /*
     * Use a server address from the command
     * line, otherwise default to 127.0.0.1:
     */
    if ( argc >= 2 )
        srvr_addr = argv[1];

    len_inet = sizeof adr_srvr;
    z = mkaddr(&adr_srvr,&len_inet,
        srvr_addr,"tcp");

    if ( z < 0 || !adr_srvr.sin_port ) {
        fprintf(stderr,"Invalid server "
            "address, or no port number "
            "was specified.\n");
        exit(1);
    }

    /*
     * Create a TDP/IP socket to use :
     */
    s = socket(PF_INET,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket(2)");

    /*
     * Bind the server address:
     */
    z = bind(s,(struct sockaddr *)&adr_srvr,
            len_inet);
    if ( z == -1 )
        bail("bind(2)");

    /*
     * Make it a listening socket:
     */
    z = listen(s,10);
    if ( z == -1 )
        bail("listen(2)");

    /*
     * Start the server loop :
     */
    for (;;) {
        /*
         * Wait for a connect :
         */
        len_inet = sizeof adr_clnt;
        c = accept(s,
            (struct sockaddr *)&adr_clnt,
            &len_inet);
        if ( c == -1 )
            bail("accept(2)");

        /*
         * Create streams :
         */
        rx = fdopen(c,"r");
        if ( !rx ) {
            /* Failed */
            close(c);
            continue;
        }

        tx = fdopen(dup(c),"w");
        if ( !tx ) {
            fclose(rx);
            continue;
        }

        /*
         * Set both streams to line
         * buffered mode :
         */
        setlinebuf(rx);
        setlinebuf(tx);

        /*
         * Process client's requests :
         */
        while ( fgets(buf,sizeof buf,rx) )
            rpn_process(tx,buf);

        /*
         * Close this client's connection:
         */
        fclose(tx);
        shutdown(fileno(rx),SHUT_RDWR);
        fclose(rx);
    }

    /* Control never gets here */
    return 0;
}
