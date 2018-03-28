/* This program was the subject of a thread in comp.lang.c, because of the way it handled EOF.
 * The complaint was that, in the event of a text file's last line not ending with a newline,
 * this program would not count the last word. I objected somewhat to this complaint, on the
 * grounds that "if it hasn't got a newline at the end of each line, it isn't a text file".
 *
 * These grounds turned out to be incorrect. Whether such a file is a text file turns out to
 * be implementation-defined. I'd had a go at checking my facts, and had - as it turns out -
 * checked the wrong facts! (sigh)
 *
 * It cost me an extra variable. It turned out that the least disturbing way to modify the
 * program (I always look for the least disturbing way) was to replace the traditional
 * while((c = getchar()) != EOF) with an EOF test actually inside the loop body. This meant
 * adding an extra variable, but is undoubtedly worth the cost, because it means the program
 * can now handle other people's text files as well as my own. As Ben Pfaff said at the
 * time, "Be liberal in what you accept, strict in what you produce". Sound advice.
 *
 * The new version has, of course, been tested, and does now accept text files not ending in
 * newlines.
 *
 * I have, of course, regenerated the sample output from this program. Actually, there's no
 * "of course" about it - I nearly forgot.
 */

#include <stdio.h>

#define MAXWORDLEN 10

int main(void)
{
  int c;
  int inspace = 0;
  long lengtharr[MAXWORDLEN + 1];
  int wordlen = 0;

  int firstletter = 1;
  long thisval = 0;
  long maxval = 0;
  int thisidx = 0;
  int done = 0;

  for(thisidx = 0; thisidx <= MAXWORDLEN; thisidx++)
  {
    lengtharr[thisidx] = 0;
  }

  while(done == 0)
  {
    c = getchar();

    if(c == ' ' || c == '\t' || c == '\n' || c == EOF)
    {
      if(inspace == 0)
      {
        firstletter = 0;
        inspace = 1;

        if(wordlen <= MAXWORDLEN)
        {
          if(wordlen > 0)
          {
            thisval = ++lengtharr[wordlen - 1];
            if(thisval > maxval)
            {
              maxval = thisval;
            }
          }
        }
        else
        {
          thisval = ++lengtharr[MAXWORDLEN];
          if(thisval > maxval)
          {
            maxval = thisval;
          }
        }
      }
      if(c == EOF)
      {
        done = 1;
      }
    }
    else
    {
      if(inspace == 1 || firstletter == 1)
      {
        wordlen = 0;
        firstletter = 0;
        inspace = 0;
      }
      ++wordlen;
    }
  }

  for(thisval = maxval; thisval > 0; thisval--)
  {
    printf("%4d  | ", thisval);
    for(thisidx = 0; thisidx <= MAXWORDLEN; thisidx++)
    {
      if(lengtharr[thisidx] >= thisval)
      {
        printf("*  ");
      }
      else
      {
        printf("   ");
      }
    }
    printf("\n");
  }
  printf("      +");
  for(thisidx = 0; thisidx <= MAXWORDLEN; thisidx++)
  {
    printf("---");
  }
  printf("\n       ");
  for(thisidx = 0; thisidx < MAXWORDLEN; thisidx++)
  {
    printf("%2d ", thisidx + 1);
  }
  printf(">%d\n", MAXWORDLEN);

  return 0;
}

