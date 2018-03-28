/* Gregory Pietsch <gkp1@flash.net> Exercise 5-2 dated 2001-01-08 */

#include <ctype.h>
#include <limits.h>

/* also uses getch and ungetch from Section 4.3 */

/* number of significant digits in a double */
#define SIG_MAX 32

/* store double in d; return next character */
int getfloat(double *d)
{
    const char point = '.';     /* localeconv->decimal_point[0]; */
    int c;
    char buf[SIG_MAX], sign, sawsign, sawe, sawesign, esign;
    double x;
    static double fac[] = {0.0, 1.0e8, 1.0e16, 1.0e24, 1.0e32};
    double dpow;
    int ndigit, nsig, nzero, olead, opoint, n;
    char *pc;
    long lo[SIG_MAX / 8 + 1], lexp;
    long *pl;

    /* skip white space */
    while (isspace(c = getch()))
        ;
    if (sawsign = (c == '-' || c == '+')) {
        sign = c;
        c = getch();
    } else
        sign = '+';
    olead = -1;
    opoint = -1;
    ndigit = 0;
    nsig = 0;
    nzero = 0;
    while (c != EOF) {
        if (c == point) {
            if (0 <= opoint)
                break;  /* already seen point */
            else
                opoint = ndigit;
        } else if (c == '0') {
            /* saw a zero */
            nzero++;
            ndigit++;
        } else if (!isdigit(c))
            break;      /* found nondigit */
        else {
            /* got a nonzero digit */
            if (olead < 0)
                olead = nzero;
            else {
                /* deliver zeros */
                for ( ; 0 < nzero && nsig < SIG_MAX; --nzero)
                    buf[nsig++] = 0;
            }
            ++ndigit;
            /* deliver digit */
            if (nsig < SIG_MAX)
                buf[nsig++] = (c - '0');
        }
        c = getch();
    }
    if (ndigit == 0) {
        /* no digits? */
        *d = 0.0;
        if (c != EOF)
            ungetch(c);
        if (0 <= opoint) {
            /* saw point */
            ungetch(c = point);
        }
        if (sawsign) {
            /* saw sign */
            ungetch(c = sign);
        }
        return c;
    }
    /* skip trailing digits */
    for ( ; 0 < nsig && buf[nsig - 1] == 0; --nsig)
        ;
    /* compute significand */
    pc = buf;
    pl = &(lo[nsig >> 3]);
    for (*pl = 0, n = nsig; 0 < n; --n) {
        if ((n & 7) == 0)
            /* start new sum */
            *--pl = *pc++;
        else
            *pl = *pl * 10 + *pc++;
    }
    for (*d = (double)(lo[0]), n = 0; ++n <= (nsig >> 3); )
        if (lo[n] != 0)
           *d += fac[n] * (double)(lo[n]);
    /* fold in any explicit exponent */
    lexp = 0;
    if (c == 'e' || c == 'E') {
        /* we have an explicit exponent */
        sawe = c;
        c = getch();
        if (sawesign = (c == '+' || c == '-')) {
            esign = c;
            c = getch();
        } else
            esign = '+';
        if (!isdigit(c)) {
            /* ill-formed exponent */
            if (c != EOF)
                ungetch(c);
            if (sawesign)
                ungetch(c = esign);
            c = sawe;
        } else {
            /* get exponent */
            while (isdigit(c)) {
                /* get explicit exponent digits */
                if (lexp < 100000)
                    lexp = lexp * 10 + (c - '0');
                /* else overflow */
                c = getch();
            }
            if (esign == '-')
                lexp = -lexp;
        }
    }
    if (c != EOF)
        ungetch(c);
    if (opoint < 0)
        lexp += ndigit - nsig;
    else
        lexp += opoint - olead - nsig;
    /* this is where I pray I don't lose precision */
    esign = (lexp < 0) ? '-' : '+';
    /* if anyone has a better way of handling overflow, tell me */
    if (lexp < SHRT_MIN)
        lexp = SHRT_MIN;
    if (lexp > SHRT_MAX)
        lexp = SHRT_MAX;
    if (lexp < 0)
        lexp = -lexp;
    if (lexp != 0) {
        dpow = (esign == '-') ? 0.1 : 10.0;
        while (lexp != 0) {
            /* form 10.0 to the lexp power */
            if ((lexp & 1) != 0) /* lexp is positive */
                *d *= dpow;
            lexp >>= 1;
            dpow *= dpow;
        }
    }
    /* if there was a minus sign in front, negate *d */
    if (sign == '-')
        *d = -(*d);
    return c;
}


