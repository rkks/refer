#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long hchartoi (char hexdig, int pos);   /* converts a hex char to decimal knowing its 0 based place value */
long htoi (char hexstring[]);               /* converts a string of hex bits to integer ... */

int main(void)
{
  char *endp = NULL;
  char *test[] =
  {
    "F00",
    "bar",
    "0100",
    "0x1",
    "0XA",
    "0X0C0BE",
    "abcdef",
    "123456",
    "0x123456",
    "deadbeef",
    "zog_c"
  };

  long int result;
  long int check;

  size_t numtests = sizeof test / sizeof test[0];

  size_t thistest;

  for(thistest = 0; thistest < numtests; thistest++)
  {
    result = htoi(test[thistest]);
    check = strtol(test[thistest], &endp, 16);

    if((*endp != '\0' && result == -1) || result == check)
    {
      printf("Testing %s. Correct. %ld\n", test[thistest], result);
    }
    else
    {
      printf("Testing %s. Incorrect. %ld\n", test[thistest], result);
    }
  }

  return 0;
}

long htoi (char s[])
{

    char *p = &s[strlen(s)-1];
    long deci = 0, dig = 0;
    int pos = 0;

    while (p >= s) {

        if ((dig = hchartoi(*p, pos)) < 0 ) {
            printf("Error\n");
            return -1;

        }
        deci += dig;
        --p;
        ++pos;

    }
    return deci;
}

/* convert hex char to decimal value */
long hchartoi (char hexdig, int pos)
{

    char hexdigits[] = "0123456789ABCDEF";
    char *p = &hexdigits[0];
    long deci = 0;
    int i;

    while (*p != toupper(hexdig) && deci < 16) {

        ++p;
        ++deci;

    }
    if (*p == toupper(hexdig)) {
       for (i = 0; i < pos; i++)
           deci *= 16;
       return deci;

    }
    return -1;
}
