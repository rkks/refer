/* Exercise 3.57: Write a program that checks whether a given string is a
 * palindrome, ignoring blanks. For example, your program should report
 * success for the string `if i had a hifi`.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  char *input = argv[1];
  int i, i2, inputlen = strlen(input);
  char *input_without_blanks = malloc((inputlen+1)*sizeof(char));

  printf("input: %s\n", input);
  for (i = 0, i2 = 0; input[i] != 0; ++i)
    if (input[i] != ' ')
      input_without_blanks[i2++] = input[i];
  input_without_blanks[i2--] = 0;

  for (i = 0; i <= i2/2; ++i) {
    if (input_without_blanks[i] != input_without_blanks[i2-i]) {
      printf("no, not a palindrome.\n");
      return 0;
    }
  }
  printf("yes, a palindrome.\n");
  free(input_without_blanks);
  return 0;
}
