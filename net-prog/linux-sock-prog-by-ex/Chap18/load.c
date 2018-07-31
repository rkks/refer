/*
 * Load tickers from tickets.rc :
 */
#include "quotes.h"

int
load(TickReq *tick,int *pntick,int nmax) {
    int ntick = *pntick;
    FILE *tickf = NULL;
    char *cp;
    char buf[256];
    int lno = 0;

    memset(tick,0,sizeof *tick * ntick);

    /*
     * Open tickers.rc file :
     */
    tickf = fopen(TICKPATH,"r");
    if ( !tickf ) {
        msgf('e',"%s: opening %s for reading",
            strerror(errno),TICKPATH);
        return -1;
    }

    /*
     * Load tickers from file :
     */
    while ( fgets(buf,sizeof buf,tickf) ) {
        ++lno;
        cp = strtick(buf);
        if ( cp - buf <= 0 ) {
            msgf('e',"Line %d: Bad ticker\n",
                lno);
            continue;
        }

        /*
         * Load one more ticker :
         */
        *cp = 0;
        strncpy(tick[ntick].ticker,
            buf, TICKLEN)[TICKLEN] = 0;
        tick[ntick].flags = 0;
        tick[ntick++].next_samp = 0;

        /*
         * Don't load beyond the array limits:
         */
        if ( ntick >= nmax ) {
            msgf('w',
                "Stopping at max of %d tickers.",
                ntick);
            break;
        }
    }

    fclose(tickf);
    msgf('i',"%d tickers loaded.",ntick);
    *pntick = ntick;    /* Return count */

    return 0;
}
