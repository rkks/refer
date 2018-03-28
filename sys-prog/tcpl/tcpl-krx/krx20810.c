/* K&R exercise 2-8 

It is class 1 due to the /sizeof/ operator (CHAR_BIT is introduced with
<limits.h> in Chapter 1). I could have used the conditional operator but
thought that this is clearer.

Notes:
1. Implicit int removed (not absolutely necessary but...)
2. Checks for the size of the shift and reduces it to the range 0 -
(number of bits in an int) - 1 This is to avoid right shifting the
number into oblivion.
3. If either the value or the shift is zero then nothing need to be done
to the parameter so just return it.

*/

unsigned int rightrot(unsigned int x, unsigned int n)
{
   /* calculate number of bits in type */
   size_t s = sizeof(x) * CHAR_BIT;
   size_t p;

   /* limit shift to range 0 - (s - 1) */
   if(n < s)
       p = n; 
   else
       p = n % s;

   /* if either is zero then the original value is unchanged */
   if((0 == x) || (0 == p))
       return x;

   return (x >> p) | (x << (s - p));
}

/* Driver based on yours but runs the shift values beyond the size of an
unsigned integer on any system */

int main(void)
{
   unsigned int val;
   unsigned int pos;
   unsigned int max = sizeof (pos) * CHAR_BIT + 1;

   for(val = 0; val < 700; val += 49)
   {
      for(pos = 0; pos < max; ++pos)
      {
         printf("%u, %d: %u\n", x, n, rightrot(val, pos));
      }
   }
}

