/* K&R Exercise 4-7 */
/* Steven Huang */

#include <string.h>
#include <stdio.h>

#define BUFSIZE 100

char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */

int getch(void) /* get a (possibly pushed back) character */
{
  return (bufp > 0) ? buf[--bufp] : getchar();
}
 
void ungetch(int c) /* push character back on input */
{
  if(bufp >= BUFSIZE)
    printf("ungetch: too many characters\n");
  else  
    buf[bufp++] = c;  
}
 
/*
   ungets() actually takes a little bit of thought.  Should the
   first character in "s" be sent to ungetch() first, or should
   it be sent last?  I assumed that most code calling getch()
   would be of this form:

     char array[...];
     int i;   

     while (...) {
       array[i++] = getch();
     }                  

   In such cases, the same code might call ungets() as:

     ungets(array);

   and expect to repeat the while loop to get the same string
   back.  This requires that the last character be sent first
   to ungetch() first, because getch() and ungetch() work with 
   a stack.     

   To answer K&R2's additional question for this problem,
   it's usually preferable for something like ungets() to just
   build itself on top of ungetch().  This allows us to change 
   ungetch() and getch() in the future, perhaps to use a linked 
   list instead, without affecting ungets().
*/ 
void ungets(const char *s)
{    
  size_t i = strlen(s);

  while (i > 0)
    ungetch(s[--i]);
}
 
int main(void)
{
  char *s = "hello, world.  this is a test.";
  int c;

  ungets(s);
  while ((c = getch()) != EOF)
    putchar(c);               
  return 0;
}
