/* Exercise 3.56: Write a program that takes a string as argument, and that
 * prints out a table giving, for each character that occurs in the string, the
 * character and its frequency of occurrence.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NCHAR 96

int main(int argc, char *argv[]) {
  int i, cnt[NCHAR] = {0};
  char *input = argv[1];

  for (; *input != 0; ++input)
    cnt[*input-' ']++;

  for (i = 0; i < NCHAR; ++i)
    if (cnt[i])
      printf("%c\t", i+' ');
  printf("\n");
  for (i = 0; i < NCHAR; ++i)
    if (cnt[i])
      printf("%d\t", cnt[i]);
  printf("\n");
  return 0;
}
