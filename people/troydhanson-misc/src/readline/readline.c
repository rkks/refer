#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
int main(int argc, char *argv[]) {
  char *line;
  while ( (line = readline("prompt: ")) != NULL) {
    printf("read: %s\n",line);
    free(line);
  }
}
