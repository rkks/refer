/* mktwatch.c :
 * 
 * Get datagram stock market
 * quotes from central quotes
 * server :
 */
#include "quotes.h"

/*
 * -b option (broadcast) address :
 */
static char *cmdopt_b = DFLT_BCAST;

/*
 * Display command usage :
 */
static void
usage(void) {
    puts("Usage:\tmktwatch [-b bcast]");
    puts("where:");
    puts("\t-b bcast\tBroadcast address");
}

/*
 * Extract ticker information from
 * broadcast datagram packet:
 */
static int
extract(char *dgram,TickReq *tkr) {
    char *cp = dgram;

    memset(tkr,0,sizeof *tkr);
    strncpy(tkr->ticker,dgram,TICKLEN)
        [TICKLEN] = 0;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->last_trade) != 1 )
        return -1;
    cp += strlen(cp) + 1;
    tkr->date = cp;
    cp += strlen(cp) + 1;
    tkr->time = cp;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->change) != 1 )
        return -1;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->open_price) != 1 )
        return -1;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->high) != 1 )
        return -1;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->low) != 1 )
        return -1;
    cp += strlen(cp) + 1;
    if ( sscanf(cp,"%lE",&tkr->volume) != 1 )
        return -1;
    return 0;
}

/*
 * Market Watch Main Program :
 */
int
main(int argc,char **argv) {
    int rc = 0;     /* Command return code */
    int optch;         /* Option character */
    int z;                  /* Status code */
    int s;                       /* Socket */
    socklen_t bc_len;           /* length  */
    struct sockaddr_in bc_addr; /* AF_INET */
    socklen_t a_len;     /* Address length */
    struct sockaddr_in adr;     /* AF_INET */
    char dgram[2048];       /* Recv buffer */
    const int True = TRUE;  /* True const. */
    TickReq tkr;            /* Ticker Data */
    const char cmdopts[] = "hb:";

    /*
     * Parse command line options :
     */
    while ( (optch = getopt(argc,argv,cmdopts)) != -1 )
        switch ( optch ) {

        case 'h' :            /* -h (help) */
            usage();
            return rc;

        case 'b' :           /* -b bc_addr */
            cmdopt_b = optarg;
            break;

        default :
            /* Option error */
            rc = 1;
    }

    if ( rc ) {
        usage();          /* Option errors */
        return rc;
    }
        
    /*
     * Form broadcast address :
     */
    bc_len = sizeof bc_addr;
    z = mkaddr(
        &bc_addr,   /* Returned addr. */
        &bc_len,    /* Returned len. */
        cmdopt_b,   /* Input address */
        "udp");     /* UDP protocol */

    if ( z == -1 ) {
        fprintf(stderr,
            "%s: -b %s",
            strerror(errno),
            cmdopt_b);
        return 1;
    }

    /*
     * Create a UDP socket to read from :
     */
    s = socket(PF_INET,SOCK_DGRAM,0);
    if ( s == -1 ) {
        fprintf(stderr,
            "%s: socket(2)\n",
            strerror(errno));
        return 1;
    }

    /*
     * Allow multiple listeners on this
     * broadcast address :
     */
    z = setsockopt(s,
        SOL_SOCKET,
        SO_REUSEADDR,
        &True,
        sizeof True);
    
    if ( z == -1 ) {
        fprintf(stderr,
            "%s: setsockopt(SO_REUSEADDR)\n",
            strerror(errno));
        return 1;
    }

    /*
     * Bind to the broadcast address :
     */
    z = bind(s,
        (struct sockaddr *)&bc_addr,bc_len);

    if ( z == -1 ) {
        fprintf(stderr,
            "%s: bind(%s)\n",
            strerror(errno),
            cmdopt_b);
        return 1;
    }

    /*
     * Now listen for and process broadcasted
     * stock quotes :
     */
    for (;;) {
        /*
         * Wait for a broadcast message :
         */
        a_len = sizeof adr; /* Max addr len. */
        z = recvfrom(s,            /* Socket */
            dgram,       /* Receiving buffer */
            sizeof dgram,/* Max rcv buf size */
            0,          /* Flags: no options */
            (struct sockaddr *)&adr, /* Addr */
            &a_len);   /* Addr len, in & out */

        if ( z < 0 ) {
            fprintf(stderr,
                "%s: recvfrom(2)\n",
                strerror(errno));
            break;
        }

        /*
         * Extract and report quote :
         */
        if ( !extract(dgram,&tkr) ) {
            printf("%-*s %7.3f %s %7s %+7.3f %7.3f "
                "%7.3f %7.3f %9.0f\n",
                TICKLEN,
                tkr.ticker,
                tkr.last_trade,
                tkr.date,
                tkr.time,
                tkr.change,
                tkr.open_price,
                tkr.high,
                tkr.low,
                tkr.volume);
            fflush(stdout);
        }
    }

    return 0;
}
