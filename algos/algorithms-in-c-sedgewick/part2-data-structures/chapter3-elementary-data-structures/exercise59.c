/* Exercise 3.59: Write a program that takes a string as argument and reads a
 * sequence of words (sequences of characters separated by blank space) from
 * standard input, printing out those that appear as substrings somewhere in
 * the argument string.
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool strin(char *arg, char *word) {
  int i, j, wordlen = strlen(word), arglen = strlen(arg);
  bool result;
  for (i = 0; i <= arglen-wordlen; ++i) {
    result = true;
    for (j = 0; j < wordlen; ++j) {
      if (word[j] != arg[i+j]) {
	result = false;
        break;
      }
    }
    if (result)
      return result;
  }
  return result;
}

int main(int argc, char *argv[]) {
  char word[10000], *arg = argv[1];

  printf("search argument: %s\n", arg);
  while (scanf("%s", word) != EOF) {
    printf("word: %s\t", word);
    if (strin(arg, word))
      printf("contained");
    printf("\n");
  }
  return 0;
}
