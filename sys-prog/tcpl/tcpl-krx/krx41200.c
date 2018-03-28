/* 

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

#include <stdlib.h>

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
        u = -value;
    } else
        u = value;
    utoa(u, d, base);
    return digits;
}



