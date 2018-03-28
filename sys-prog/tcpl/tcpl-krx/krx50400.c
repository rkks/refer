/*

    Exercise 5-4. Write the function strend(s,t), which returns 1 if the string t 
                  occurs at the end of the string s, and zero otherwise.

    Author : Bryan Williams

*/


int strlen(char *s) /* added by RJH; source: K&R p99 */
{
  int n;

  for(n = 0; *s != '\0'; s++)
  {
    n++;
  }
  return n;
}

int strcmp(char *s, char *t) /* added by RJH; source: K&R p106 */
{
  for(;*s == *t; s++, t++)
    if(*s == '\0')
      return 0;
  return *s - *t;
}


int strend(char *s, char *t)
{
  int Result = 0;
  int s_length = 0;
  int t_length = 0;

  /* get the lengths of the strings */
  s_length = strlen(s);
  t_length = strlen(t);

  /* check if the lengths mean that the string t could fit at the string s */
  if(t_length <= s_length)
  {
    /* advance the s pointer to where the string t would have to start in string s */
    s += s_length - t_length;

    /* and make the compare using strcmp */
    if(0 == strcmp(s, t))
    {
      Result = 1;
    }
  }

  return Result;
}



#include <stdio.h>


int main(void)
{
  char *s1 = "some really long string.";
  char *s2 = "ng.";
  char *s3 = "ng";

  if(strend(s1, s2))
  {
    printf("The string (%s) has (%s) at the end.\n", s1, s2);
  }
  else
  {
    printf("The string (%s) doesn't have (%s) at the end.\n", s1, s2);
  }
  if(strend(s1, s3))
  {
    printf("The string (%s) has (%s) at the end.\n", s1, s3);
  }
  else
  {
    printf("The string (%s) doesn't have (%s) at the end.\n", s1, s3);
  }

  return 0;
}








