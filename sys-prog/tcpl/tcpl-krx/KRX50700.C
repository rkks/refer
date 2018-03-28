/* K&R Exercise 5-7 */
/* Steven Huang */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TRUE     1
#define FALSE    0

#define MAXLINES 5000       /* maximum number of lines */
#define MAXLEN   1000       /* maximum length of a line */
char *lineptr[MAXLINES];
char lines[MAXLINES][MAXLEN];

/* K&R2 p29 */
int getline(char s[], int lim)
{
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    s[i] = c;                                                         
  if (c == '\n') {
    s[i++] = c;   
  }
  s[i] = '\0';
  return i;
}

/* K&R2 p109 */
int readlines(char *lineptr[], int maxlines)
{
  int len, nlines;
  char *p, line[MAXLEN];

  nlines = 0;
  while ((len = getline(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (p = malloc(len)) == NULL)
      return -1;
    else {
      line[len - 1] = '\0';  /* delete the newline */
      strcpy(p, line);
      lineptr[nlines++] = p;
    }                       
  return nlines;  
}

int readlines2(char lines[][MAXLEN], int maxlines)
{
  int len, nlines;

  nlines = 0;
  while ((len = getline(lines[nlines], MAXLEN)) > 0)
    if (nlines >= maxlines)                         
      return -1;           
    else
      lines[nlines++][len - 1] = '\0';  /* delete the newline */
  return nlines;
}
 
int main(int argc, char *argv[])
{
  /* read things into cache, to be fair. */
  readlines2(lines, MAXLINES);

  if (argc > 1 && *argv[1] == '2') {
    puts("readlines2()");           
    readlines2(lines, MAXLINES);
  } else {
    puts("readlines()");
    readlines(lineptr, MAXLINES);
  }
   
  return 0;
}
