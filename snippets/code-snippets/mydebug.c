/**
 * mydebug.c -- Written by Tyler Montbriand, 2005.
 * tsm@accesscomm.ca http://burningsmell.org/
 * freeware
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "mydebug.h"

void _debug_print(const char *cmdstr, ...)
{
  va_list ap;

  va_start(ap,cmdstr);
    vfprintf(stderr,cmdstr,ap);
  va_end(ap);
}

void _info_print(const char *cmdstr, ...)
{
  va_list ap;

  /**
   * When DEBUG is undefined, it checks for the DEBUG environment variable.
   * Otherwise, it always prints.
   */

#ifndef DEBUG
  if(getenv("DEBUG") == NULL) return;
#endif

  va_start(ap,cmdstr);
    vfprintf(stderr,cmdstr,ap);
  va_end(ap);
}
