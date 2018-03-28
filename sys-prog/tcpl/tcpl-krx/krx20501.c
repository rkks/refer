#include <stdio.h> /* for NULL */

int any(char *s1, char *s2) 
{
        char array[256]; /* rjh comments
                          * (a) by making this char array[256] = {0}; the first loop becomes unnecessary.
                          * (b) for full ANSIness, #include <limits.h>, make the array unsigned char,
                          *     cast as required, and specify an array size of UCHAR_MAX + 1.
                          * (c) the return statements' (parentheses) are not required.
                          */
        int  i;
        if (s1 == NULL) {
                if (s2 == NULL) {
                        return(0);
                } else {
                        return(-1);
                }
        }

        for(i = 0; i < 256; i++) {
                array[i] = 0;
        }
        
        while(*s2 != '\0') {
                array[*s2] = 1;
                s2++;
        }

        i = 0;
        while(s1[i] != '\0') {
                if (array[s1[i]] == 1) {
                        return(i);
                }
                i++;
        }
        return(-1);
}

/* test driver by Richard Heathfield */

/* We get a helpful boost for testing from the question text, because we are
 * told that the function's behaviour is identical to strpbrk except that it
 * returns a pointer instead of a position. We use this fact to validate the
 * function's correctness.
 */

#include <string.h>

int main(void)
{
  char *leftstr[] =
  {
    "",
    "a",
    "antidisestablishmentarianism",
    "beautifications",
    "characteristically",
    "deterministically",
    "electroencephalography",
    "familiarisation",
    "gastrointestinal",
    "heterogeneousness",
    "incomprehensibility",
    "justifications",
    "knowledgeable",
    "lexicographically",
    "microarchitectures",
    "nondeterministically",
    "organizationally",
    "phenomenologically",
    "quantifications",
    "representationally",
    "straightforwardness",
    "telecommunications",
    "uncontrollability",
    "vulnerabilities",
    "wholeheartedly",
    "xylophonically",
    "youthfulness",
    "zoologically"
  };
  char *rightstr[] =
  {
    "",
    "a",
    "the",
    "quick",
    "brown",
    "dog",
    "jumps",
    "over",
    "lazy",
    "fox",
    "get",
    "rid",
    "of",
    "windows",
    "and",
    "install",
    "linux"
  };

  size_t numlefts = sizeof leftstr / sizeof leftstr[0];
  size_t numrights = sizeof rightstr / sizeof rightstr[0];
  size_t left = 0;
  size_t right = 0;

  int passed = 0;
  int failed = 0;

  int pos = -1;
  char *ptr = NULL;

  for(left = 0; left < numlefts; left++)
  {
    for(right = 0; right < numrights; right++)
    {
      pos = any(leftstr[left], rightstr[right]);
      ptr = strpbrk(leftstr[left], rightstr[right]);

      if(-1 == pos)
      {
        if(ptr != NULL)
        {
          printf("Test %d/%d failed.\n", left, right);
          ++failed;
        }
        else
        {
          printf("Test %d/%d passed.\n", left, right);
          ++passed;
        }
      }
      else
      {
        if(ptr == NULL)
        {
          printf("Test %d/%d failed.\n", left, right);
          ++failed;
        }
        else
        {
          if(ptr - leftstr[left] == pos)
          {
            printf("Test %d/%d passed.\n", left, right);
            ++passed;
          }
          else
          {
            printf("Test %d/%d failed.\n", left, right);
            ++failed;
          }
        }
      }
    }
  }
  printf("\n\nTotal passes %d, fails %d, total tests %d\n",
         passed,
         failed,
         passed + failed);
  return 0;
}
