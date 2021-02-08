/* Exercise 3.17: Write a program that counts the number of different integers
 * less than 1000 that appear in an input stream.
 */
#include <stdio.h>
#include <stdbool.h>
#define N 1000

int main() {
  int i, cnt = 0;
  long l;
  bool arr[N];

  for (i = 0; i < N; ++i)
    arr[i] = 0;

  while (scanf("%ld", &l) != EOF) {
    if (l < 1000 && !arr[l]) {
      ++cnt;
      arr[l] = 1;
    }
    printf("number: %ld\tcount: %d\n", l, cnt);
  }
  return 0;
}
