/*

  Exercise 7-1. Write a program that converts upper case to lower case or lower case to upper, 
                depending on the name it is invoked with, as found in argv[0].


  Assumptions: The program should read from stdin, until EOF, converting the output to stdout
               appropriately. 

               The correct outputs should be :
 
               Program Name                Output
               lower                       stdin with all caps converted to lower case
               upper                       stdin with all lowercase characters converted to uppercase
               [anything else]             helpful message explaining how to use this

  Author : Bryan Williams

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SUCCESS           0
#define NO_ARGV0          1
#define BAD_NAME          2


int main(int argc, char *argv[])
{
  int ErrorStatus = SUCCESS;
  int (*convert)(int c) = NULL;
  int c = 0;
   
  /* check that there were any arguments */
  if(SUCCESS == ErrorStatus)
  {
    if(0 >= argc)
    {
      printf("Your environment has not provided a single argument for the program name.\n");
      ErrorStatus = NO_ARGV0;
    }
  }

  /* check for valid names in the argv[0] string */
  if(SUCCESS == ErrorStatus)
  {
    if(0 == strcmp(argv[0], "lower"))
    {
      convert = tolower;
    } 
    else if(0 == strcmp(argv[0], "upper"))
    {
      convert = toupper;
    }
    else
    {
      printf("This program performs two functions.\n");
      printf("If the executable is named lower then it converts all the input on stdin to lowercase.\n");
      printf("If the executable is named upper then it converts all the input on stdin to uppercase.\n");
      printf("As you have named it %s it prints this message.\n", argv[0]);
      ErrorStatus = BAD_NAME;
    }
  }

  /* ok so far, keep looping until EOF is encountered */
  if(SUCCESS == ErrorStatus)
  {
    while(EOF != (c = getchar()))
    {
      putchar((*convert)(c));
    }
  }

  /* and return what happened */
  return SUCCESS == ErrorStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}




