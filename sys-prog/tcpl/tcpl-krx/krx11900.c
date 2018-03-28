#include <stdio.h>

#define MAX_LINE 1024

void discardnewline(char s[])
{
  int i;
  for(i = 0; s[i] != '\0'; i++)
  {
    if(s[i] == '\n')
      s[i] = '\0';
  }
}

int reverse(char s[])
{
  char ch;
  int i, j;

  for(j = 0; s[j] != '\0'; j++)
  {
  }

  --j;

  for(i = 0; i < j; i++)
  {
    ch   = s[i];
    s[i] = s[j];
    s[j] = ch;
    --j;
  }

  return 0;
}

int getline(char s[], int lim)
{
  int c, i;

  for(i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
  {
    s[i] = c;
  }

  if(c == '\n')
  {
    s[i++] = c;
  }

  s[i] = '\0';

  return i;

}

int main(void)
{
  char line[MAX_LINE];

  while(getline(line, sizeof line) > 0)
  {
    discardnewline(line);
    reverse(line);
    printf("%s\n", line);
  }
  return 0;
}
