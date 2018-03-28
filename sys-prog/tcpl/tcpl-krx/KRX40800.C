/* K&R Exercise 4-8 */
/* Steven Huang */

#include <stdio.h>

int buf = EOF; /* buffer for ungetch */

int getch(void) /* get a (possibly pushed back) character */
{
  int temp;

  if (buf != EOF) {
    temp = buf;
    buf = EOF;
  } else {
    temp = getchar();
  }
  return temp;                          
}
 
void ungetch(int c) /* push character back on input */
{
  if(buf != EOF)
    printf("ungetch: too many characters\n");
  else       
    buf = c;
}
 
int main(void)
{
  int c;

  while ((c = getch()) != EOF) {
    if (c == '/') {
      putchar(c);
      if ((c = getch()) == '*') { 
        ungetch('!');
      }         
    } 
    putchar(c);               
  }
  return 0;
}

