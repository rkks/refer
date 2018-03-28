/* K&R2 1-18 p31: Write a program to remove trailing blanks and tabs
   from each line of input, and to delete entirely blank lines.

   The program specification is ambiguous: does "entirely blank lines"
   mean lines that contain no characters other than newline, or does
   it include lines composed of blanks and tabs followed by newline?
   The latter interpretation is taken here.

   This implementation does not use any features not introduced in the
   first chapter of K&R2.  As a result, it can't use pointers to
   dynamically allocate a buffer to store blanks that it has seen, so
   it must limit the number of blanks that are allowed to occur
   consecutively.  (This is the value of MAXQUEUE, minus one.)

   It is intended that this implementation "degrades gracefully."
   Even though a particular input might have 1000 or more trailing
   blanks or tabs in a row, causing a problem for a single pass,
   multiple passes through the file will correct the problem.  The
   program signals the need for such an additional pass by returning a
   failure code to the operating system.  (EXIT_FAILURE isn't mentioned
   in the first chapter of K&R, but I'm making an exception here.) */

#include <stdio.h>
#include <stdlib.h>

#define MAXQUEUE 1001

int advance(int pointer)
{
  if (pointer < MAXQUEUE - 1)
    return pointer + 1;
  else
    return 0;
}

int main(void)
{
  char blank[MAXQUEUE];
  int head, tail;
  int nonspace;
  int retval;
  int c;
  int spaceJustPrinted; /*boolean: was the last character printed whitespace?*/

  retval = spaceJustPrinted = nonspace = head = tail = 0;

  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      head = tail = 0;
      if (spaceJustPrinted == 1) /*if some trailing whitespace was printed...*/
        retval = EXIT_FAILURE;

      if (nonspace) {
        putchar('\n');
        spaceJustPrinted = 0; /* this instruction isn't really necessary since
                              spaceJustPrinted is only used to determine the
                              return value, but we'll keep this boolean
                              truthful */
        nonspace = 0;  /* moved inside conditional just to save a needless
                       assignment */
      }
    }
    else if (c == ' ' || c == '\t') {
      if (advance(head) == tail) {
        putchar(blank[tail]); /* these whitespace chars being printed early
                              are only a problem if they are trailing,
                              which we'll check when we hit a \n or EOF */
        spaceJustPrinted = 1;
        tail = advance(tail);
        nonspace = 1;
      }

      blank[head] = c;
      head = advance(head);
    }
    else {
      while (head != tail) {
        putchar(blank[tail]);
        tail = advance(tail);
      }
      putchar(c);
      spaceJustPrinted = 0;
      nonspace = 1;
    }
  }

  /* if the last line wasn't ended with a newline before the EOF,
  we'll need to figure out if trailing space was printed here */
  if (spaceJustPrinted == 1) /*if some trailing whitespace was printed...*/ 
    retval = EXIT_FAILURE;

  return retval;
}


