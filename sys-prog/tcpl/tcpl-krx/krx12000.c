#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER   1024
#define SPACE        ' '
#define TAB          '\t'

int CalculateNumberOfSpaces(int Offset, int TabSize)
{
   return TabSize - (Offset % TabSize);
}

/* K&R's getline() function from p29 */
int getline(char s[], int lim)
{
  int c, i;

  for(i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
    s[i] = c;
  if(c == '\n')
  {
    s[i] = c;
    ++i;
  }
  s[i] = '\0';

  return i;
}

int main(void)
{
  char  Buffer[MAX_BUFFER];
  int TabSize = 5; /* A good test value */

  int i, j, k, l;

  while(getline(Buffer, MAX_BUFFER) > 0)
  {
    for(i = 0, l = 0; Buffer[i] != '\0'; i++)
    {
      if(Buffer[i] == TAB)
      {
        j = CalculateNumberOfSpaces(l, TabSize);
        for(k = 0; k < j; k++)
        {
          putchar(SPACE);
          l++;
        }
      }
      else
      {
        putchar(Buffer[i]);
        l++;
      }
    }
  }

  return 0;
}

