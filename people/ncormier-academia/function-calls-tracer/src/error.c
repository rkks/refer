/*
** error.c for  in /home/nico/lang/c/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Thu Mar 23 15:57:19 2006 nicolas
** Last update Sat Mar 25 21:54:46 2006 nicolas
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

void	errexit(char *format, ...)
{
  va_list	ap;
  char		buf[BUFSIZ];

  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s\n", buf);
  va_end(ap);
  exit(1);
}

static char*	__errstr;

char**	__ftrace_errstr(void)
{
  return (&__errstr);
}

char*	ftrace_strerror(int errno)
{
  if (errno == EFTRACE)
    return (__errstr);
  return (strerror(errno));
}
