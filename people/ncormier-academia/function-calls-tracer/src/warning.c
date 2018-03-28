/*
** warning.c for  in /home/nico/lang/c/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Thu Mar 23 15:57:19 2006 nicolas
** Last update Thu Apr  6 11:37:59 2006 nicolas
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "warning.h"

void	pwarn(char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s\n", buf);
  va_end(ap);
}
