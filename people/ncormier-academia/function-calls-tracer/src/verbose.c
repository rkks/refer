/*
** verbose.c for  in /home/nico/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Sun Apr  9 21:06:16 2006 nicolas
** Last update Sun Apr  9 21:36:22 2006 nicolas
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verbose.h"

extern int	verbose;

void	pverbose(int verbose_level, char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  if (verbose < verbose_level)
    return;
  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stdout, "%s", buf);
  va_end(ap);
}
