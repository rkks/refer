/* Andrew Tesker
 *
 * krx30102.c
 */

#include <stdio.h>

/* find x in v[] */
int binsearch(int x, int v[], int n);

/*
  The main is here for the purpose of a built in test
  
 */


int main(void)
{
  int test[]={1,3,5,7,9,11,13};
  int i;
  for(i=(sizeof(test)/sizeof(int))-1; i>=0; --i)
    printf("looking for %d. Index=%d\n",test[i],binsearch(test[i], test, sizeof(test)/sizeof(*test)));

  return 0;

}
/* n = size of array v */

int binsearch(int x, int v[], int n)
{
  int low, high, mid;

  low = 0;
  high = n-1;
  
  while(low < high) {
    mid = (low+high)/2;
    if(x <= v[mid]) 
      high=mid;
    
    else 
      low = mid+1;
  }
  
  return (x == v[low])?low : -1;

}


