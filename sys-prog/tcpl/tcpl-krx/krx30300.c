/*

  EX3_3.C
  =======

  Suggested solution to Exercise 3-3

*/

#include <stdio.h>
#include <string.h>

void expand(char * s1, char * s2);

int main(void) {
    char *s[] = { "a-z-", "z-a-", "-1-6-",
                  "a-ee-a", "a-R-L", "1-9-1",
                  "5-5", NULL };
    char result[100];
    int i = 0;
    
    while ( s[i] ) {
        
        /*  Expand and print the next string in our array s[]  */
        
        expand(result, s[i]);
        printf("Unexpanded: %s\n", s[i]);
        printf("Expanded  : %s\n", result);
        ++i;
    }
    
    return 0;
}


/*  Copies string s2 to s1, expanding
    ranges such as 'a-z' and '8-3'     */

void expand(char * s1, char * s2) {
    static char upper_alph[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static char lower_alph[27] = "abcdefghijklmnopqrstuvwxyz";
    static char digits[11]     = "0123456789";
    
    char * start, * end, * p;
    int i = 0;
    int j = 0;
    
    
    /*  Loop through characters in s2  */
    
    while ( s2[i] ) {
        switch( s2[i] ) {
        case '-':
            if ( i == 0 || s2[i+1] == '\0' ) {
                
                /*  '-' is leading or trailing, so just copy it  */
                
                s1[j++] = '-';
                ++i;
                break;
            }
            else {
                
                 /*  We have a "range" to extrapolate. Test whether
                     the two operands are part of the same range. If
                     so, store pointers to the first and last characters
                     in the range in start and end, respectively. If 
                     not, output and error message and skip this range.    */
                
                if ( (start = strchr(upper_alph, s2[i-1])) &&
                     (end   = strchr(upper_alph, s2[i+1])) )
                    ;
                else if ( (start = strchr(lower_alph, s2[i-1])) &&
                          (end   = strchr(lower_alph, s2[i+1])) )
                    ;
                else if ( (start = strchr(digits, s2[i-1])) &&
                          (end   = strchr(digits, s2[i+1])) )
                    ;
                else {
                    
                    /*  We have mismatched operands in the range,
                        such as 'a-R', or '3-X', so output an error
                        message, and just copy the range expression.  */
                    
                    fprintf(stderr, "EX3_3: Mismatched operands '%c-%c'\n",
                            s2[i-1], s2[i+1]);
                    s1[j++] = s2[i-1];
                    s1[j++] = s2[i++];
                    break;
                }
                
                
                /*  Expand the range  */
                
                p = start;
                while ( p != end ) {
                    s1[j++] = *p;
                    if ( end > start )
                        ++p;
                    else
                        --p;
                }
                s1[j++] = *p;
                i += 2;
            }
            break;
            
        default:
            if ( s2[i+1] == '-' && s2[i+2] != '\0' ) {
                
                /*  This character is the first operand in
                    a range, so just skip it - the range will
                    be processed in the next iteration of
                    the loop.                                   */
                
                ++i;
            }
            else {
                
                /*  Just a normal character, so copy it  */
                
                s1[j++] = s2[i++];
            }
            break;
        }
    }
    s1[j] = s2[i];    /*  Don't forget the null character  */
}

