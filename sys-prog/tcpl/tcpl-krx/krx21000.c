/*

  Exercise 2-10. Rewrite the function lower, which converts upper case letters
		 to lower case, with a conditional expression instead of if-else.



  Assumptions : by conditional expression they mean an expression involving a ternary operator.

  Author: Bryan Williams 

*/

#include <stdio.h>
#include <string.h>

#define TEST
#define ORIGINAL            0
#define SOLUTION            1
#define PORTABLE_SOLUTION   0


/*
   ok, the original routine we are trying to convert looks like this.. 
*/
#if ORIGINAL
/* lower: convert c to lower case; ASCII only */
int lower(int c)
{
  if(c >= 'A' && c <= 'Z')
    return c + 'a' - 'A';
  else
    return c;
}
#endif

/*
   the natural solution for simply making this a conditional (ternary) return instead of an 
   if ... else ...
*/
#if SOLUTION
/* lower: convert c to lower case; ASCII only */
int lower(int c)
{
  return c >= 'A' && c <= 'Z' ? c + 'a' - 'A' : c;
}
#endif



/*
   the more portable solution, requiring string.h for strchr but keeping the idea of a 
   conditional return.  
*/
#if PORTABLE_SOLUTION
/* lower: convert c to lower case */
int lower(int c)
{
  char *Uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *Lowercase = "abcdefghijklmnopqrstuvwxyz";
  char *p = NULL;

  return NULL == (p = strchr(Uppercase, c)) ? c : *(Lowercase + (p - Uppercase));
}
#endif



/*
    ok, this bit is just a test driver... exclude as required
*/
#ifdef TEST
int main(void)
{
  char *Tests = "AaBbcCD3EdFGHgIJKLhM2NOjPQRkSTlUVWfXYf0Z1";
  char *p = Tests;
  int Result = 0;  

  while('\0' != *p)
  {
    Result = lower(*p);
    printf("[%c] gives [%c]\n", *p, Result);
    ++p;
  }

  /* and the obligatory boundary test */
  Result = lower(0);
  printf("'\\0' gives %d\n", Result); 

  return 0;
}

#endif

