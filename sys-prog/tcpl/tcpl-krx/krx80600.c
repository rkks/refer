/*
    Exercise 8.6. The standard library function calloc(n, size) returns a pointer to n objects
		  of size size, with the storage initialised to zero. Write calloc, by calling 
		  malloc or by modifying it.

    Author: Bryan Williams

*/

#include <stdlib.h>
#include <string.h>

/*
  Decided to re-use malloc for this because :
	1) If the implementation of malloc and the memory management layer changes, this will be ok.
	2) Code re-use is great.

*/
void *mycalloc(size_t nmemb, size_t size)
{
  void *Result = NULL;

  /* use malloc to get the memory */
  Result = malloc(nmemb * size);

  /* and clear the memory on successful allocation */
  if(NULL != Result)
  {
    memset(Result, 0x00, nmemb * size);
  }

  /* and return the result */
  return Result;
}

/* simple test driver, by RJH */

#include <stdio.h>

int main(void)
{
  int *p = NULL;
  int i = 0;

  p = mycalloc(100, sizeof *p);
  if(NULL == p)
  {
    printf("mycalloc returned NULL.\n");
  }
  else
  {
    for(i = 0; i < 100; i++)
    {
      printf("%08X ", p[i]);
      if(i % 8 == 7)
      {
	printf("\n");
      }
    }
    printf("\n");
    free(p);
  }

  return 0;
}


