/* gettick.c
 *
 * Get ticker info from inet:
 */
#include "quotes.h"

/*
 * f is set TRUE when a request
 * for a stock quote has timed
 * out.
 */
static int f = FALSE;

/*
 * Catch SIGALRM and Timeout :
 */
static void
sig_ALRM(int signo) {
    f = TRUE;   /* Mark timeout */
}

/*
 * Get ticker info :
 *
 * RETURNS:
 *  0       Success
 *  -1      Failed:
 *
 * errno:
 *  ETIME   Timed Out
 *  EBADMSG Field data format
 *  other   Network/system errors.
 */
int
get_tickinfo(TickReq *req,char *addr) {
    int z, er;          /* Status, errno */
    int s;              /* Socket */
    int n;              /* Byte count */
    char buf[256];      /* Receive buffer */
    char *tkr = NULL;   /* Extracted ticker */
    struct sigaction
        sa_new,         /* New signal action */
        sa_old;         /* Saved signal action */
    Parm parms[9];      /* Data parse table */

    /*
     * Initialize parsing parameters. This
     * parameter list will need modification
     * if yahoo or your quote provider uses
     * a different format:
     */
    parms[0].type = 'S';    /* String */
    parms[0].parm = &tkr;   /* Ticker name */
    parms[1].type = 'D';    /* Double */
    parms[1].parm = &req->last_trade;
    parms[2].type = 'S';
    parms[2].parm = &req->date;
    parms[3].type = 'S';
    parms[3].parm = &req->time;
    parms[4].type = 'D';
    parms[4].parm = &req->change;
    parms[5].type = 'D';
    parms[5].parm = &req->open_price;
    parms[6].type = 'D';
    parms[6].parm = &req->high;
    parms[7].type = 'D';
    parms[7].parm = &req->low;
    parms[8].type = 'D';
    parms[8].parm = &req->volume;

    /*
     * Initialize to catch SIGALRM :
     */
    sa_new.sa_handler = sig_ALRM;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;
    sigaction(SIGALRM,&sa_new,&sa_old);
        
    /*
     * Connect to finance.yahoo.com :
     */
    f = FALSE;
    alarm(TIMEOUT_SECS);

    s = Connect(addr);
    if ( s == -1 )
        goto errxit;

    /*
     * Send GET request :
     *
     * NOTE: This is subject to change-
     * If finance.yahoo.com changes, you
     * will need to adjust this formatting.
     */
    sprintf(buf,"GET /d/quotes.csv?"
        "s=%s"
        "&f=sl1d1t1c1ohgv"
        "&e=.csv\r\n",
        req->ticker);

    write(s,buf,strlen(buf));
    shutdown(s,1);

    /*
     * Read response with a timeout:
     */
    do  {
        z = read(s,buf,sizeof buf);
    } while ( !f && z == -1 && errno == EINTR );

    er = errno;              /* Save error */
    alarm(0);           /* Disable timeout */
    close(s);              /* Close socket */

    /* Restore the signal action */
    sigaction(SIGALRM,&sa_old,NULL);

    if ( !f && z > 0 )
        n = z;          /* Read n bytes OK */
    else {
        if ( f )               /* Timeout? */
            er = ETIME;    /* Yes- timeout */
        /*
         * Report error to log:
         */
        msgf('e',"%s: Get ticker '%s'",
            strerror(er),
            req->ticker);

        errno = er;          /* For caller */
        return -1;               /* Failed */
    }

    /* Remove CR, LF, or CRLF */
    buf[strcspn(buf,"\r\n")] = 0;

    /*
     * Check for the unknown ticker case :
     */
    if ( strstr(buf,"N/A,N/A,N/A,N/A,N/A") ) {
        msgf('e',"Unknown Ticker: '%s'",
            req->ticker);
        req->flags |= FLG_UNKNOWN;
        errno = EBADMSG;    /* For caller */
        return -1;          /* Failed */
    }

    /*
     * Parse quote results:
     */
    if ( (z = extract_parms(parms,9,buf)) < 0 ) {
        /* Report failed parse of data */
        msgf('e',"Field # %d: '%s'",z,buf);
        req->flags |= FLG_ERROR;
        errno = EBADMSG;    /* For caller */
        return -1;          /* Failed */
    }

    /* Capture the exact case for this ticker */
    strncpy(req->ticker,tkr,TICKLEN)[TICKLEN] = 0;

    /*
     * Update sample time in entry :
     */
    return 0;

    /*
     * Error Exit:
     */
errxit:
    alarm(0);
    sigaction(SIGALRM,&sa_old,NULL);
    return -1;
}
