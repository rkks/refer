/**
 * C implimentations of the popular pushd/popd functions.
 *
 * Written by Tyler Montbriand.  Freeware.
 */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "dirstack.h"

#define DIR_STACK_SIZE 16
static int dirpos=0;
static char *dir_stack[DIR_STACK_SIZE];

int pushd(const char *fname)
{
  char buf[512];
  if(dirpos>=(DIR_STACK_SIZE-1))
    return(-1);

  getcwd(buf,511);
  buf[511]='\0';

  if(chdir(fname)<0)
    return(-1);

  dir_stack[dirpos]=strdup(buf);
  dirpos++;
  return(0);
}

int popd()
{
  if(dirpos<=0)
    return(-1);

  chdir(dir_stack[dirpos-1]);
  free(dir_stack[dirpos-1]);
  dirpos--;
  return(0);
}
