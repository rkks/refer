/**
 * Removes whitespace from the end of a string.
 *
 * Written by Tyler Montbriand.  Freeware.
 */
#include <string.h>
#include <ctype.h>
#include "chomp.h"

void chomp(char *str)
{
  int len=strlen(str);

  if(len>0)
  while((len>0) && isspace(str[len-1]))
    str[--len]='\0';
}

