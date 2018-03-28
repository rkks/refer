#include <stdio.h>

int main(void)
{
  int blanks, tabs, newlines;
  int c;
  int done = 0;
  int lastchar = 0;

  blanks = 0;
  tabs = 0;
  newlines = 0;

  while(done == 0)
  {
    c = getchar();

    if(c == ' ')
      ++blanks;

    if(c == '\t')
      ++tabs;

    if(c == '\n')
      ++newlines;

    if(c == EOF)
    {
      if(lastchar != '\n')
      {
        ++newlines; /* this is a bit of a semantic stretch, but it copes
                     * with implementations where a text file might not
                     * end with a newline. Thanks to Jim Stad for pointing
                     * this out.
                     */
      }
      done = 1;
    }
    lastchar = c;
  }

  printf("Blanks: %d\nTabs: %d\nLines: %d\n", blanks, tabs, newlines);
  return 0;
}

