/**
 * debugtest.c -- Written by Tyler Montbriand, 2005
 * tsm@accesscomm.ca http://burningsmell.org/
 * freeware
 */
#include "mydebug.h"

int main()
{
  DEBUG_PRINT(("This should only print when DEBUG's defined in preprocessor %d\n",5));
  INFO_PRINT(("If DEBUG is defined, this always prints\n"));
  INFO_PRINT(("If DEBUG isn't defined, it only prints if DEBUG is in the environment\n"));
  return(0);
}
