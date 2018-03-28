/* K&R Exercise 5-14 */
/* Steven Huang */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MAXLINES 5000       /* maximum number of lines */
char *lineptr[MAXLINES];

#define MAXLEN 1000         /* maximum length of a line */

int reverse = FALSE;

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

/* K&R2 p109 */
void writelines(char *lineptr[], int nlines)
{
  int i;

  for (i = 0; i < nlines; i++)
    printf("%s\n", lineptr[i]);
}

int pstrcmp(const void *p1, const void *p2)
{
  char * const *s1 = reverse ? p2 : p1;
  char * const *s2 = reverse ? p1 : p2;

  return strcmp(*s1, *s2);
}

int numcmp(const void *p1, const void *p2)
{
  char * const *s1 = reverse ? p2 : p1;
  char * const *s2 = reverse ? p1 : p2;
  double v1, v2;

  v1 = atof(*s1);
  v2 = atof(*s2);
  if (v1 < v2)
    return -1;
  else if (v1 > v2)                                     
    return 1;      
  else
    return 0;
}

int main(int argc, char *argv[])
{
  int nlines;
  int numeric = FALSE;
  int i;

  for (i = 1; i < argc; i++) {
    if (*argv[i] == '-') {
      switch (*(argv[i] + 1)) {
        case 'n':  numeric = TRUE;  break;
        case 'r':  reverse = TRUE;  break;
        default:
          fprintf(stderr, "invalid switch '%s'\n", argv[i]);
          return EXIT_FAILURE;
      }
    }
  }

  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    qsort(lineptr, nlines, sizeof(*lineptr), numeric ? numcmp : pstrcmp);
    writelines(lineptr, nlines);
    return EXIT_SUCCESS;
  } else {
    fputs("input too big to sort\n", stderr);
    return EXIT_FAILURE;
  }
}

