/* Test driver by Richard Heathfield
 * Solution (strrindex function) by Rick Dearman
 */

#include <stdio.h>

/* Write the function strrindex(s,t), which returns the position
** of the rightmost occurrence of t in s, or -1 if there is none.
*/

int strrindex( char s[], char t )
{
  int i;
  int count = -1;

  for(i=0; s[i] != '\0'; i++)
  {
    if(s[i] == t)
    {
      count = i;
    }
  }

  return count;
}


typedef struct TEST
{
  char *data;
  char testchar;
  int expected;
} TEST;

int main(void)
{
  TEST test[] =
  {
    {"Hello world", 'o', 7},
    {"This string is littered with iiiis", 'i', 32},
    {"No 'see' letters in here", 'c', -1}
  };

  size_t numtests = sizeof test / sizeof test[0];
  size_t i;

  char ch = 'o';
  int pos;

  for(i = 0; i < numtests; i++)
  {
    pos = strrindex(test[i].data, test[i].testchar);

    printf("Searching %s for last occurrence of %c.\n",
           test[i].data,
           test[i].testchar);

    printf("Expected result: %d\n", test[i].expected);
    printf("%sorrect (%d).\n", pos == test[i].expected ? "C" : "Inc", pos);
    if(pos != -1)
    {
      printf("Character found was %c\n", test[i].data[pos]);
    }
  }

  return 0;
}

