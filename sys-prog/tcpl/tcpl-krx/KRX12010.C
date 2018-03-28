#include <stdio.h>
#include <stdlib.h>

#define TAB 7

int main(void) {
  int c,i;

  i = NULL;

  while ((c = getchar()) != EOF) {
    i++;
    if (c == '\n')
      i = NULL; /* reset column counter */
    if (c == '\t') {
      while ((i % TAB) != 0) {
        putchar(' ');
        i++;
      }
    } else {
      putchar(c);
    }

  }
  return(0);
}
