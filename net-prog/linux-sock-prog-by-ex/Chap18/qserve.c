/* qserve.c :
 *
 * Stock Quote Concentrator Program :
 */
#include "quotes.h"

static char *command = NULL;

/* Remote Quote Server Address */
static char *cmdopt_a = DFLT_SERVER;

/* Quote Re-Broadcast Address */
static char *cmdopt_b = DFLT_BCAST;

/*
 * Ticker Table :
 */
static TickReq tickers[MAX_TICKERS];
static int ntick = 0;

/*
 * Return server usage information :
 */
static void
usage(void) {
    printf("Usage: %s [-h] [-a address:port]\n"
        "where:\n"
        "\t-h\t\tRequests usage info.\n"
        "\t-a address:port\tSpecify "
        "the server\n"
        "\t\t\taddress and port number.\n"
        "\t-b bcast:port\tSpecify "
        "the broadcast\n"
        "\t\t\taddress and port number.\n",
        command);
}

/*
 * Server Main Program :
 */
int
main(int argc,char **argv) {
    int rc = 0;             /* Return Code */
    int optch;             /* Option Char. */
    int z;                  /* Status Code */
    int x;                        /* Index */
    int s;             /* Broadcast socket */
    time_t tn = 0;            /* Time Next */
    time_t zzz;              /* Sleep Time */
    time_t tm = 20;             /* Seconds */
    time_t td;              /* Time & Date */
    struct sockaddr_in bc_addr; /* bc addr */
    socklen_t bc_len;      /* bc addr len. */
    const int True = TRUE;  /* Const. TRUE */
    static char cmdopts[] = "ha:b:";

    /*
     * Process command line options :
     */
    command = Basename(argv[0]);

    while ( (optch = getopt(argc,argv,cmdopts)) != -1 )
        switch ( optch ) {

        case 'h' :          /* -h for help */
            usage();
            return rc;

        case 'a' :      /* -a quote_server */
            cmdopt_a = optarg;
            break;

        case 'b' :    /* -b broadcast_addr */
            cmdopt_b = optarg;
            break;

        default :
            /* Option error */
            rc = 1;
    }

    /*
     * Check for option errors :
     */
    if ( rc ) {
        usage();
        return rc;
    }
        
    /*
     * Form the broadcast server
     * address:
     */
    bc_len = sizeof bc_addr;    /* Max len */
    z = mkaddr(
        &bc_addr,        /* Returned addr. */
        &bc_len,          /* Returned len. */
        cmdopt_b,         /* Input address */
        "udp");            /* UDP protocol */

    if ( z == -1 ) {
        msgf('e',"%s: -b %s",
            strerror(errno),
            cmdopt_b);
        return 1;
    }

    /*
     * Create a UDP socket to use :
     */
    s = socket(PF_INET,SOCK_DGRAM,0);

    if ( s == -1 ) {
        msgf('e',"%s: socket(PF_INET,"
            "SOCK_DGRAM,0)",
            strerror(errno));
        return 1;
    }

    /*
     * Allow broadcasts on socket s :
     */
    z = setsockopt(s,
        SOL_SOCKET,
        SO_BROADCAST,
        &True,
        sizeof True);

    if ( z == -1 ) {
        msgf('e',"%s: setsockopt(SO_BROADCAST)",
            strerror(errno));
        return 1;
    }

    /*
     * Load tickers from tickers.rc :
     */
    if ( load(&tickers[0],&ntick,MAX_TICKERS) )
        goto errxit;

    /*
     * Now monitor the remote quote server :
     */
    for (;;) {
        tn = 0;              /* Refresh tn */
        time(&td);         /* Current time */

        /*
         * Loop for all tickers :
         */
        for ( x=0; x<ntick; ++x ) {
            /*
             * Skip tickers that are either
             * unknown, or are producing parse
             * errors in the returned data :
             */
            if ( tickers[x].flags & FLG_UNKNOWN
              || tickers[x].flags & FLG_ERROR )
                continue;   /* Ignore this */

            /*
             * Pick up the earliest "next" time:
             */
            if ( !tn
              || tickers[x].next_samp < tn )
                tn = tickers[x].next_samp;

            /*
             * If the current time is > than
             * the "next" time, it is time to
             * fetch an update for this ticker:
             */
            if ( td >= tickers[x].next_samp ) {
                /*
                 * Get Quote Update :
                 */
                z = get_tickinfo(
                    &tickers[x],cmdopt_a);

                /*
                 * Compute time for the next
                 * update for this ticker :
                 */
                time(&tickers[x].next_samp);
                tickers[x].next_samp += tm;

                /*
                 * If the quote fetch was OK,
                 * then broadcast its info :
                 */
                if ( !z )
                    broadcast(s,&tickers[x],
                        (struct sockaddr *)&bc_addr,
                        bc_len);
            }
        }

        /*
         * Here the interval between updates is
         * progressively increased to 5 minutes
         * max. This provides a lot of initial
         * action for demonstration purposes,
         * without taxing the friendly quote
         * providers if this program is run all
         * day. Abuse will only force the kind
         * providers to change things to break
         * the operation of this program!
         */
        if ( tm < (time_t) 5 * 60 )
            tm += 5;    /* Progressively increase */

        /*
         * Compute how long we need to snooze.
         * The time to the next event is
         * computed- sleep(3) is called if
         * necessary:
         */
        if ( !tn )
            tn = td + tm;
        if ( tn >= td )
            if ( (zzz = tn - td) )
                sleep(zzz);
    }

    return rc;

    /*
     * Error Exit :
     */
errxit:
    return rc = 2;
}
