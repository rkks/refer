 /* Program 3.17 Sorting an array of strings
  * This program illustrates an an important string-processing function:
  * rearranging a set of strings into sorted order. We read strings into a
  * buffer large enough to hold them all, maintaining a pointer to each string
  * in an array, then rearrange the pointers to put the pointer to the smallest
  * string in the first position in the array, the pointer to the second
  * smallest string in the second position in the array, and so forth.
  *
  * The qsort library function that actually does the sort takes four arguments:
  * a pointer to the beginning of the array, the number of objects, the size of
  * each object, and a comparison function. It achieves independence from the
  * type of object being sorted by blindly rearranging the blocks of data that
  * represent objects (in this case string pointers) and by using a comparison
  * function that takes pointers to void as argument. This code casts these back
  * to type pointer to pointer to char for strcmp. To actually access the first
  * character in a string for a comparison, we dereference three pointers: one
  * to get the index (which is a pointer) into our array, one to get the pointer
  * to the string (using the index), and one to get the character (using the pointer).
  *
  * We use a different method to achieve type independence for our
  * sorting and searching functions (see Chapters 4 and 6).
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Nmax 1000
#define Mmax 10000

char buf[Mmax];
int M = 0;

int compare(void *i, void *j) {
  return strcmp(*(char **) i, *(char **) j);
}

int main() {
  int i, N;
  char *a[Nmax];

  for (N = 0; N < Nmax; ++N) {
    a[N] = &buf[M];
    if (scanf("%s", a[N]) == EOF)
      break;
    M += strlen(a[N])+1;
  }
  qsort(a, N, sizeof(char *), compare);
  for (i = 0; i < N; ++i)
    printf("%s\n", a[i]);
  return 0;
}
