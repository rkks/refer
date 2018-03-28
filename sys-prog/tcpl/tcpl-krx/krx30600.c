/*

  EX3_6.C
  =======
  
  Suggested solution to Exercise 3-6
  
*/

#include <stdio.h>
#include <limits.h>

void itoa(int n, char s[], int width);
void reverse(char s[]);

int main(void) {
    char buffer[20];
    
    itoa(INT_MIN, buffer, 7);
    printf("Buffer:%s\n", buffer);
    
    return 0;
}

void itoa(int n, char s[], int width) {
    int i, sign;
    
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
        printf("%d %% %d + '0' = %d\n", n, 10, s[i-1]);
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    
    while (i < width )    /*  Only addition to original function  */
        s[i++] = ' ';
    
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) {
    int c, i, j;
    for ( i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

