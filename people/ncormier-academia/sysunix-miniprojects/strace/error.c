/*
** error.c for  in /home/nico/lang/c/strace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Sun Jan  1 17:53:04 2006 nicolas
** Last update Thu Jan  5 13:10:29 2006 nicolas cormier
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void		errexit(char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s\n", buf);
  va_end(ap);
  exit(1);
}
