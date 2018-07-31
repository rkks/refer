/* srvr2.c:
 * 
 * Example daytime server,
 * with gethostbyaddr(3) :
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
    char *srvr_addr = NULL;
    char *srvr_port = "9099";
    struct sockaddr_in adr_srvr;/* AF_INET */
    struct sockaddr_in adr_clnt;/* AF_INET */
    int len_inet;               /* length  */
    int s;                       /* Socket */
    int c;                /* Client socket */
    int n;                        /* bytes */
    time_t td;        /* Current date&time */
    char dtbuf[128];     /* Date/Time info */
    FILE *logf; /* Log file for the server */
    struct hostent *hp;  /* Host entry ptr */

    /*
     * Open the log file :
     */
    if ( !(logf = fopen("srvr2.log","w")) )
        bail("fopen(3)");

    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.1 :
     */
    if ( argc >= 2 ) {
        /* Addr on cmdline: */
        srvr_addr = argv[1];
    } else {
        /* Use default address: */
        srvr_addr = "127.0.0.1";
    }

    /*
     * If there is a second argument on the
     * command line, use it as the port # :
     */
    if ( argc >= 3 )
        srvr_port = argv[2];

    /*
     * Create a TDP/IP socket to use :
     */
    s = socket(PF_INET,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Create a server socket address:
     */
    memset(&adr_srvr,0,sizeof adr_srvr);
    adr_srvr.sin_family = AF_INET;
    adr_srvr.sin_port = htons(atoi(srvr_port));
    if ( strcmp(srvr_addr,"*") != 0 ) {
        /* Normal Address */
        adr_srvr.sin_addr.s_addr =
            inet_addr(srvr_addr);
        if ( adr_srvr.sin_addr.s_addr
             == INADDR_NONE )
            bail("bad address.");
    } else {
        /* Wild Address */
        adr_srvr.sin_addr.s_addr =
            INADDR_ANY;
    }

    /*
     * Bind the server address:
     */
    len_inet = sizeof adr_srvr;
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
         * Log the address of the client
         * who connected to us:
         */
        fprintf(logf,
            "Client %s:",
            inet_ntoa(adr_clnt.sin_addr));

        hp = gethostbyaddr(
            (char *)&adr_clnt.sin_addr,
            sizeof adr_clnt.sin_addr,
            adr_clnt.sin_family);

        if ( !hp )
            fprintf(logf," Error: %s\n",
                hstrerror(h_errno));
        else
            fprintf(logf," %s\n",
                hp->h_name);
        fflush(logf);

        /*
         * Generate a time stamp :
         */
        time(&td);
        n = (int) strftime(dtbuf,sizeof dtbuf,
            "%A %b %d %H:%M:%S %Y\n",
            localtime(&td));

        /*
         * Write result back to the client :
         */
        z = write(c,dtbuf,n);
        if ( z == -1 )
            bail("write(2)");

        /*
         * Close this client's connection:
         */
        close(c);
    }

    /* Control never gets here */
    return 0;
}
