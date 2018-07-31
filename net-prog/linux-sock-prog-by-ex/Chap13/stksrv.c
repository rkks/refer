/* stksrv.c:
 * 
 * Example Stock Index Broadcast :
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

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

extern int mkaddr(
    void *addr,
    int *addrlen,
    char *str_addr,
    char *protocol);

#define MAXQ    4

static struct {
    char    *index;
    int     start;
    int     volit;
    int     current;
} quotes[] = {
    { "DJIA",    1030330, 375 },
    { "NASDAQ",  276175,  125 },
    { "S&P 500", 128331,   50 },
    { "TSE 300", 689572,   75 },
};

/*
 * Initialize :
 */
static void
initialize(void) {
    short x;
    time_t td;

    /*
     * Seed the random number generator :
     */
    time(&td);
    srand((int)td);

    for ( x=0; x < MAXQ; ++x )
        quotes[x].current =
            quotes[x].start;
}

/*
 * Randomly change one index quotation :
 */
static void
gen_quote(void) {
    short x;    /* Index */
    short v;    /* Volatility of index */
    short h;    /* Half of v */
    short r;    /* Random change */

    x = rand() % MAXQ;
    v = quotes[x].volit;
    h = (v / 2) - 2;
    r = rand() % v;
    if ( r < h )
        r = -r;
    quotes[x].current += r;
}

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    short x;     /* index of Stock Indexes */
    double I0;      /* Initial index value */
    double I;               /* Index value */
    char bcbuf[512], *bp;/* Buffer and ptr */
    int z;           /* Status return code */
    int s;                       /* Socket */
    struct sockaddr_in adr_srvr;/* AF_INET */
    int len_srvr;               /* length  */
    struct sockaddr_in adr_bc;  /* AF_INET */
    int len_bc;                 /* length  */
    static int so_broadcast = TRUE;
    static char
        *sv_addr = "127.0.0.1:*",
        *bc_addr = "127.255.255.255:9097";

    /*
     * Form a server address :
     */
    if ( argc > 2 )
        /* Server address : */
        sv_addr = argv[2];

    if ( argc > 1 )
        /* Broadcast address : */
        bc_addr = argv[1];

    /*
     * Form the server address :
     */
    len_srvr = sizeof adr_srvr;

    z = mkaddr(
        &adr_srvr,  /* Returned address */
        &len_srvr,  /* Returned length */
        sv_addr,    /* Input string addr */
        "udp");     /* UDP protocol */

    if ( z == -1 )
        bail("Bad server address");

    /*
     * Form the broadcast address :
     */
    len_bc = sizeof adr_bc;

    z = mkaddr(
        &adr_bc,    /* Returned address */
        &len_bc,    /* Returned length */
        bc_addr,    /* Input string addr */
        "udp");     /* UDP protocol */

    if ( z == -1 )
        bail("Bad broadcast address");

    /*
     * Create a UDP socket to use :
     */
    s = socket(AF_INET,SOCK_DGRAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Allow broadcasts :
     */
    z = setsockopt(s,
        SOL_SOCKET,
        SO_BROADCAST,
        &so_broadcast,
        sizeof so_broadcast);

    if ( z == -1 )
        bail("setsockopt(SO_BROADCAST)");

    /*
     * Bind a address to our socket, so that
     * client programs can listen to this
     * server:
     */
    z = bind(s,
        (struct sockaddr *)&adr_srvr,
        len_srvr);

    if ( z == -1 )
        bail("bind()");

    /*
     * Now start serving quotes :
     */
    initialize();

    for (;;) {
        /*
         * Update one quote in the list:
         */
        gen_quote();

        /*
         * Form a packet to send out :
         */
        bp = bcbuf;
        for ( x=0; x<MAXQ; ++x ) {
            I0 = quotes[x].start / 100.0;
            I = quotes[x].current / 100.0;
            sprintf(bp,
                "%-7.7s %8.2f %+.2f\n",
                quotes[x].index,
                I,
                I - I0);
            bp += strlen(bp);
        }
    
        /*
         * Broadcast the updated info:
         */
        z = sendto(s,
            bcbuf,
            strlen(bcbuf),
            0,
            (struct sockaddr *)&adr_bc,
            len_bc);

        if ( z == -1 )
            bail("sendto()");

        sleep(4);
    }

    return 0;
}
