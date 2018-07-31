/* csvparse.c :
 *
 * Parse a *.CSV format record
 */
#include "quotes.h"

/*
 * Internal buffer pointers :
 */
static char *sbuf = NULL;
static unsigned maxlen = 0;
static char *aptr = NULL;

/*
 * Reset the internal alloc()
 * function:
 *
 * ARGUMENTS:
 *  nbytes  No. of bytes max
 */
static void
reset_alloc(unsigned nbytes) {

    if ( maxlen < nbytes ) {
        if ( sbuf )
            free(sbuf);
        sbuf = (char *)malloc(nbytes);
        maxlen = nbytes;
    }
    aptr = sbuf;
}

/*
 * Allocate space from sbuf:
 *
 * ARGUMENTS:
 *  nbytes  No. of bytes for this
 *          allocation request.
 *
 * RETURNS:
 *  ptr     To n bytes
 */
static char *
alloc(unsigned nbytes) {
    char *rp = aptr;

    aptr += nbytes;
    if ( aptr - sbuf > maxlen )
        abort();
    return rp;
}

/*
 * Allocate, copy/edit a string :
 *
 * ARGUMENTS:
 *  start   Start string ptr
 *  end     NULL or End string ptr
 *  qc      == 0 : no edit (just copy)
 *          != 0 : Edit out doubled
 *                 quote characters.
 * RETURNS:
 *  allocated string pointer
 */
static char *
stredit(char *start,char *end,char qc) {
    unsigned n;
    char *str;
    char *cp, *dp;

    /*
     * Insist on an end point :
     */
    if ( !end )
        end = start + strlen(start);
    n = end - start;         /* Needed */
    dp = str = alloc(n + 1); /* Allocate */

    /*
     * Just copy if no quote char :
     */
    if ( !qc ) {
        strncpy(str,start,n)[n] = 0;
        return str;
    }

    /*
     * Must edit if quote char :
     */
    for ( cp=start; cp < end; ++cp )
        /* Test for doubled quote char */
        if ( *cp == qc && cp[1] == qc ) {
            *dp++ = qc;
            ++cp;
        } else
            *dp++ = *cp;
    *dp = 0;        

    return str;
}

/*
 * Extract one field's data :
 *
 * ARGUMENTS:
 *  scanp   Ptr to a scan pointer
 *          Updated to point to
 *          next comma (or NULL
 *          if no more data)
 * RETURNS:
 *  pointer To allocated string
 *          data that was extracted.
 */
static char *
extract_field(char **scanp) {
    char *start = *scanp;
    char qc = *start;
    char *p;

    if ( qc != '"'
      && qc != '\'' ) {
        /*
         * Non-quoted field :
         */
        *scanp = strchr(start,',');
        return stredit(start,*scanp,0);
    }

    /*
     * Quoted field :
     */
    for ( p = ++start; *p; p += 2 ) {
        if ( !(p = strchr(p,qc)) )
            break;
        if ( p[1] != qc )
            break;
    }

    if ( p )
        *scanp = strchr(p,',');
    else
        *scanp = NULL;

    return stredit(start,p,qc);
}

/*
 * Extract one spreadsheet list:
 */
int
extract_parms(Parm *plist,short n,char *src) {
    short x;
    char *scanp = src;
    char *fp, *ep, **sp;
    double dval;
    
    /*
     * Prepare buffer management :
     */
    reset_alloc(strlen(src)+1);

    /*
     * Now extract all parameters :
     */
    for ( x=0; x<n && scanp; ++x ) {
        fp = extract_field(&scanp);

        switch ( plist[x].type ) {

        case 'S' :  /* String Type */
            sp = (char **)(plist[x].parm);
            if ( *sp )
                free(*sp);
            *sp = strdup(fp);
            break;

        case 'D' :  /* Double Type */
            dval = strtod(fp,&ep);
            if ( ep && *ep )
                goto errxit;
            if ( dval == 0.0
            ||   dval == +HUGE_VAL
            ||   dval == -HUGE_VAL ) {
                if ( errno == ERANGE )
                    goto errxit;
            }
            *(double *)(plist[x].parm) = dval;
            break;

        default :   /* Shouldn't Get Here */
            abort();
        }

        if ( scanp )
            ++scanp;
    }

    if ( x == n )
        return 0;   /* Completely successful */

errxit:
    return 1 + x;   /* Error in field # 1+x */
}
