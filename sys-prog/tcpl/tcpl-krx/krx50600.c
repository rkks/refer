/* Gregory Pietsch ex. 5-6 dated 2001-01-29 */

#include <ctype.h>
#include <stdio.h>
#include <strlib.h>

/* getline: get line into s, return length */
int getline(char *s, int lim)
{
    char *p;
    int c;

    p = s;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *p++ = c;
    if (c == '\n')
        *p++ = c;
    *p = '\0';
    return (int)(p - s);
}

/* atoi: convert s to an integer
 *
 * Here's the easy way:
 * int atoi(char *s){return (int)strtoul(s, NULL, 10);}
 * But I'll behave...
 */
int atoi(char *s)
{
    int n, sign;

    while (isspace(*s))
        s++;
    sign = (*s == '+' || *s == '-') ? ((*s++ == '+') ? 1 : -1) : 1;
    for (n = 0; isdigit(*s); s++)
        n = (n * 10) + (*s - '0');  /* note to language lawyers --
                                     * the digits are in consecutive
                                     * order in the character set
                                     * C90 5.2.1
                                     */
    return sign * n;
}

/* Shamelessly copied from my 4-12 answer

itoa() is non-standard, but defined on p.64 as having this prototype:

void itoa(int n, char s[])

Instead of this, I thought I'd use a different prototype (one I got from

the library manual of one of my compilers) since it includes all of the
above:

char *itoa(int value, char *digits, int base);

Description:  The itoa() function converts an integer value into an
ASCII string of digits.  The base argument specifies the number base for

the conversion.  The base must be a value in the range [2..36], where 2
is binary, 8 is octal, 10 is decimal, and 16 is hexadecimal.  The buffer

pointed to by digits must be large enough to hold the ASCII string of
digits plus a terminating null character.  The maximum amount of buffer
space used is the precision of an int in bits + 2 (one for the sign and
one for the terminating null).

Returns:  digits, or NULL if error.

*/

char *utoa(unsigned value, char *digits, int base)
{
    char *s, *p;

    s = "0123456789abcdefghijklmnopqrstuvwxyz"; /* don't care if s is in

                                                 * read-only memory
                                                 */
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < (unsigned) base) {
        digits[0] = s[value];
        digits[1] = '\0';
    } else {
        for (p = utoa(value / ((unsigned)base), digits, base);
             *p;
             p++);
        utoa( value % ((unsigned)base), p, base);
    }
    return digits;
}

char *itoa(int value, char *digits, int base)
{
    char *d;
    unsigned u; /* assume unsigned is big enough to hold all the
                 * unsigned values -x could possibly be -- don't
                 * know how well this assumption holds on the
                 * DeathStation 9000, so beware of nasal demons
                 */

    d = digits;
    if (base == 0)
        base = 10;
    if (digits == NULL || base < 2 || base > 36)
        return NULL;
    if (value < 0) {
        *d++ = '-';
        u = -((unsigned)value);
    } else
        u = value;
    utoa(u, d, base);
    return digits;
}

/* reverse, shamelessly copied from my 4-13 answer */

static void swap(char *a, char *b, size_t n)
{
    while (n--) {
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
        a++;
        b++;
    }
}

void my_memrev(char *s, size_t n)
{
    switch (n) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:
        swap(s, s + n - 1, 1);
        break;
    default:
        my_memrev(s, n / 2);
        my_memrev(s + ((n + 1) / 2), n / 2);
        swap(s, s + ((n + 1) / 2), n / 2);
        break;
    }
}

void reverse(char *s)
{
    char *p;

    for (p = s; *p; p++)
        ;
    my_memrev(s, (size_t)(p - s));
}

/* strindex: return index of t in s, -1 if not found */

/* needed strchr(), so here it is: */

static char *strchr(char *s, int c)
{
    char ch = c;

    for ( ; *s != ch; ++s)
        if (*s == '\0')
            return NULL;
    return s;
}

int strindex(char *s, char *t)
{
    char *u, *v, *w;

    if (*t == '\0')
        return 0;
    for (u = s; (u = strchr(u, *t)) != NULL; ++u) {
        for (v = u, w = t; ; )
            if (*++w == '\0')
                return (int)(u - s);
            else if (*++v != *w)
                break;
    }
    return -1;
}

/* getop */

#define NUMBER '0'      /* from Chapter 4 */

int getop(char *s)
{
    int c;

    while ((*s = c = getch()) == ' ' || c == '\t')
        ;
    *(s + 1) = '\0';
    if (!isdigit(c) && c != '.')
        return c;       /* not a number */
    if (isdigit(c))     /* collect integer part */
        while (isdigit(*++s = c = getch()))
            ;
    if (c == '.')       /* collect fraction part */
        while (isdigit(*++s = c = getch()))
            ;
    *++s = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

/* Is there any more? */

