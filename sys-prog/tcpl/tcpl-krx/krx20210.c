#include <stdio.h>

#define lim 80

int main()
{
        int i, c;
        char s[lim];

        /* There is a sequence point after the first operand of ?: */

        for(i=0; i<lim-1 ? (c=getchar()) != '\n' ? c != EOF : 0 : 0 ; ++i)
                s[i] = c;
        
        return s[i] ^= s[i]; /* null terminate and return. */
}

