/* Exercise 4.41: Write a client that tests your deque ADTs
 * (Exercise 4.37) by reading, as the first argument on the
 * command line, a string of commands like those given in
 * Exercise 4.34 then performing the indicated operations.
 * Add a function `DQdump` to the interface and
 * implementations, and print out the contents of the deque
 * after each operation, in the style of Figure 4.6.
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ItemChar.h"
#include "DEQUE.h"

int main(int argc, char *argv[]) {
  Item *args = argv[1];
  Item c;
  int args_len = strlen(args);

  DEQUEinit(args_len);

  do {
    if (*args == '*')
      c = DEQUEpop();
    else if (*args == '+')
      c = DEQUEget();
    else if (islower(*args)) {
      DEQUEappend(*args);
      c = ' ';
    }
    else if (isupper(*args)) {
      DEQUEprepend(*args);
      c = ' ';
    }
    DQdump(*args, c);
  }
  while (*(++args) != '\0');

  DEQUEdestroy();
  return 0;
}
