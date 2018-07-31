/* rpnsrv2.c:
 * 
 * Example RPN Server
 * using select(2) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <gmp.h>

#ifndef SHUT_RDWR
#define SHUT_RDWR 3
#endif

extern int mkaddr(void *addr,
    int *addr_len,
    char *input_address,
    char *protocol);

extern void rpn_process(FILE *tx,
    char *buf);

extern void rpn_free(mpz_t **v);

#define MAX_STACK       32
#define MAX_CLIENTS     64

/*
 * Declared in rpneng2.c :
 */
extern mpz_t **stack;
extern int sp;

/*
 * Client context Info:
 */
typedef struct {
    mpz_t   **stack;  /* Stack Array */
    int     sp;         /* Stack ptr */
    FILE    *rx;        /* Recv FILE */
    FILE    *tx;        /* Xmit FILE */
} ClientInfo;

ClientInfo client[MAX_CLIENTS];

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

/*
 * Process client c :
 */
static int
process_client(int c) {
    char buf[4096];          /* I/O Buffer */
    FILE *rx = client[c].rx;
    FILE *tx = client[c].tx;

    /*
     * Install correct RPN stack :
     */
    stack = client[c].stack;
    sp = client[c].sp;

    /*
     * If not EOF, process one line :
     */
    if ( !feof(rx)
      && fgets(buf,sizeof buf,rx) )
        rpn_process(tx,buf);

    if ( !feof(rx) ) {
        /* Save SP and exit */
        client[c].sp = sp;
        return 0;
    }

    /*
     * Close this client's connection:
     */
    fclose(tx);
    shutdown(fileno(rx),SHUT_RDWR);
    fclose(rx);

    client[c].rx = client[c].tx = NULL;

    while ( sp > 0 )
        rpn_free(&stack[--sp]);
    free(stack);

    client[c].stack = NULL;
    client[c].sp = 0;

    return EOF;
}

/*
 * Main program :
 */
int
main(int argc,char **argv) {
    int z;
    char *srvr_addr = "127.0.0.1:9090";
    struct sockaddr_in adr_srvr;/* AF_INET */
    struct sockaddr_in adr_clnt;/* AF_INET */
    int len_inet;               /* length  */
    int s = -1;                  /* Socket */
    int c = -1;           /* Client socket */
    int n;    /* return val from select(2) */
    int mx;                  /* Max fd + 1 */
    fd_set rx_set;             /* Read set */
    fd_set wk_set;          /* Working set */
    struct timeval tv;    /* Timeout value */

    /*
     * Initialize client structure:
     */
    for ( z=0; z<MAX_CLIENTS; ++z ) {
        client[z].stack = NULL;
        client[z].sp = 0;
        client[z].rx = NULL;
        client[z].tx = NULL;
    }

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
     * Express interest in socket
     * s for read events :
     */
    FD_ZERO(&rx_set);   /* Init. */
    FD_SET(s,&rx_set);    /* + s */
    mx = s + 1;    /* max fd + 1 */

    /*
     * Start the server loop :
     */
    for (;;) {
        /*
         * Copy the rx_set to wk_set :
         */
        FD_ZERO(&wk_set);
        for ( z=0; z<mx; ++z ) {
            if ( FD_ISSET(z,&rx_set) )
                FD_SET(z,&wk_set);
        }

        /*
         * Sample timeout of 2.03 secs
         */
        tv.tv_sec = 2;
        tv.tv_usec = 30000;

        n = select(mx,&wk_set,NULL,NULL,&tv);
        if ( n == -1 ) {
            fprintf(stderr,"%s: select(2)\n",
                strerror(errno));
            exit(1);
        } else if ( !n ) {
            /* puts("Timeout."); */
            continue;
        }

        /*
         * Check if a connect has occured:
         */
        if ( FD_ISSET(s,&wk_set) ) {
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
             * See if we've exceeded server
             * capacity. If so, close the
             * socket and wait for the
             * next event :
             */
            if ( c >= MAX_CLIENTS ) {
                close(c);   /* At capacity */
                continue;
            }

            /*
             * Create streams :
             */
            client[c].rx = fdopen(c,"r");
            if ( !client[c].rx ) {
                close(c);  /* Failed */
                continue;
            }

            client[c].tx = fdopen(dup(c),"w");
            if ( !client[c].tx ) {
                fclose(client[c].rx);
                continue;
            }

            if ( c + 1 > mx )
                mx = c + 1;

            /*
             * Set both streams to line
             * buffered mode :
             */
            setlinebuf(client[c].rx);
            setlinebuf(client[c].tx);

            /*
             * Allocate a stack :
             */
            client[c].sp = 0;
            client[c].stack =
                (mpz_t **) malloc(
                    sizeof (mpz_t *)
                    * MAX_STACK);

            FD_SET(c,&rx_set);
        } 

        /*
         * Check for client activity :
         */
        for ( c=0; c<mx; ++c ) {
            if ( c == s )
                continue;   /* Not s */
            if ( FD_ISSET(c,&wk_set) ) {
                if ( process_client(c) == EOF ) {
                    FD_CLR(c,&rx_set);
                }
            }
        }

        /*
         * Reduce mx if we are able to :
         */
        for ( c = mx - 1;
              c >= 0 && !FD_ISSET(c,&rx_set);
              c = mx - 1 )
                mx = c;
    }

    /* Control never gets here */
    return 0;
}
