/* K&R Exercise 5-13 */
/* Steven Huang */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_NUM_LINES      10
#define MAX_LINE_LEN           1000

/*
   Points of interest for a novice:

   1.  atoi() has a normally annoying property of not being able to
       tell the caller conclusively whether the input was bad ("abc")
       or it was really zero ("0"), because it returns 0 for both    
       cases.  Here, we exploit that property, because we only want
       to accept options in the form of "-n".                      

   2.  Try to understand how this program deals with input that
       doesn't even have as many lines as the line_ptrs[] array.
       That is, how does this program degenerate into just displaying
       everything it read?  (Hint:  what does it mean when line_ptrs[x]
       is NULL?)                                                       

   3.  Using modulo arithmetic on an index to a circular array is
       a common and useful technique.  Try to understand the range      
       of values that current_line (and j, later) will take.  In
       particular, why shouldn't we just do this:

       for (i = 0; i < num_lines; i++)
         if (line_ptrs[i])    
           printf("%s", line_ptrs[i]);

   4.  Why do we still use a "%s" to display what's inside line_ptrs,
       rather than just:

       printf(line_ptrs[i]);
   
   5.  There is a bug in this program, where you see:

       numlines = -numlines;

       When will this break?
*/ 

/* K&R2 p29 */
int getline(char s[], int lim)
{
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    s[i] = c;
  if (c == '\n')
    s[i++] = c;
  s[i] = '\0';
  return i;
}

/* duplicates a string */
char *dupstr(const char *s)
{
  char *p = malloc(strlen(s) + 1); 

  if (p)
    strcpy(p, s);
  return p;
}

int main(int argc, char *argv[])
{
  int num_lines = DEFAULT_NUM_LINES;
  char **line_ptrs;
  char buffer[MAX_LINE_LEN];
  int i;
  unsigned j, current_line;

  if (argc > 1) {                                                      
    /*           
       We use a little trick here.  The command line parameter should be
       in the form of "-n", where n is the number of lines.  We don't   
       check for the "-", but just pass it to atoi() anyway, and then     
       check if atoi() returned us a negative number.               
    */                                               
    num_lines = atoi(argv[1]);
    if (num_lines >= 0) {     
      fprintf(stderr, "Expected -n, where n is the number of lines\n");
      return EXIT_FAILURE;                                           
    }
    /* Now make num_lines the positive number it's supposed to be. */
    num_lines = -num_lines;                                          
  } 

   /* First, let's get enough storage for a list of n pointers... */
  line_ptrs = malloc(sizeof *line_ptrs * num_lines);
  if (!line_ptrs) {
    fprintf(stderr, "Out of memory.  Sorry.\n");
    return EXIT_FAILURE;
  }
  /* and make them all point to NULL */
  for (i = 0; i < num_lines; i++)
    line_ptrs[i] = NULL;

  /* Now start reading */
  current_line = 0;
  do {
    getline(buffer, sizeof buffer);
    if (!feof(stdin)) {
      if (line_ptrs[current_line]) {
        /* there's already something here */
        free(line_ptrs[current_line]);
      }
      line_ptrs[current_line] = dupstr(buffer);
      if (!line_ptrs[current_line]) {
        fprintf(stderr, "Out of memory.  Sorry.\n");
        return EXIT_FAILURE;
      }
      current_line = (current_line + 1) % num_lines;
    }
  } while (!feof(stdin));

  /* Finished reading the file, so we are ready to print the lines */
  for (i = 0; i < num_lines; i++) {
    j = (current_line + i) % num_lines;
    if (line_ptrs[j]) {
      printf("%s", line_ptrs[j]);
      free(line_ptrs[j]);
    }
  }

  return EXIT_SUCCESS;
}

