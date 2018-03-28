#include <stdio.h>

int main(void)
{
  int c;
  int inspace;

  inspace = 0;
  while((c = getchar()) != EOF)
  {
    if(c == ' ')
    {
      if(inspace == 0)
      {
        inspace = 1;
        putchar(c);
      }
    }

    /* We haven't met 'else' yet, so we have to be a little clumsy */
    if(c != ' ')
    {
      inspace = 0;
      putchar(c);
    }
  }

  return 0;
}

