#include <stdio.h>

/* NUM_CHARS should really be CHAR_MAX but K&R haven't covered that at this stage in the book */
#define NUM_CHARS 256

int main(void)
{
  int c;
  long freqarr[NUM_CHARS + 1];

  long thisval = 0;
  long maxval = 0;
  int thisidx = 0;

  for(thisidx = 0; thisidx <= NUM_CHARS; thisidx++)
  {
    freqarr[thisidx] = 0;
  }

  while((c = getchar()) != EOF)
  {
    if(c < NUM_CHARS)
    {
      thisval = ++freqarr[c];
      if(thisval > maxval)
      {
        maxval = thisval;
      }
    }
    else
    {
      thisval = ++freqarr[NUM_CHARS];
      if(thisval > maxval)
      {
        maxval = thisval;
      }
    }
  }

  for(thisval = maxval; thisval > 0; thisval--)
  {
    printf("%4d  |", thisval);
    for(thisidx = 0; thisidx <= NUM_CHARS; thisidx++)
    {
      if(freqarr[thisidx] >= thisval)
      {
        printf("*");
      }
      else if(freqarr[thisidx] > 0)
      {
        printf(" ");
      }
    }
    printf("\n");
  }
  printf("      +");
  for(thisidx = 0; thisidx <= NUM_CHARS; thisidx++)
  {
    if(freqarr[thisidx] > 0)
    {
      printf("-");
    }
  }
  printf("\n       ");
  for(thisidx = 0; thisidx < NUM_CHARS; thisidx++)
  {
    if(freqarr[thisidx] > 0)
    {
      printf("%d", thisidx / 100);
    }
  }
  printf("\n       ");
  for(thisidx = 0; thisidx < NUM_CHARS; thisidx++)
  {
    if(freqarr[thisidx] > 0)
    {
      printf("%d", (thisidx - (100 * (thisidx / 100))) / 10 );
    }
  }
  printf("\n       ");
  for(thisidx = 0; thisidx < NUM_CHARS; thisidx++)
  {
    if(freqarr[thisidx] > 0)
    {
      printf("%d", thisidx - (10 * (thisidx / 10)));
    }
  }
  if(freqarr[NUM_CHARS] > 0)
  {
    printf(">%d\n", NUM_CHARS);
  }

  printf("\n");

  return 0;
}

