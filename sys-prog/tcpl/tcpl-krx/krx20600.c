#include <stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
  return (x & ((~0 << (p + 1)) | (~(~0 << (p + 1 - n))))) | ((y & ~(~0 << n)) << (p + 1 - n));
}

int main(void)
{
  unsigned i;
  unsigned j;
  unsigned k;
  int p;
  int n;
  
  for(i = 0; i < 30000; i += 511)
  {
    for(j = 0; j < 1000; j += 37)
    {
      for(p = 0; p < 16; p++)
      {
        for(n = 1; n <= p + 1; n++)
        {
          k = setbits(i, p, n, j);
          printf("setbits(%u, %d, %d, %u) = %u\n", i, p, n, j, k);
        }
      }
    }
  }
  return 0;
}

