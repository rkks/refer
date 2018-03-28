/* vfprintf.c -- implement vfprintf() for architectures without it
   Copyright (C) 2000 Gary V. Vaughan
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>

#if HAVE_STDARG_H && !HAVE_VARARGS_H
#  include <stdarg.h>
#else /*!HAVE_STDARG_H || HAVE_VARARGS_H*/
#  include <varargs.h>
#endif /*HAVE_STDARG_H && !HAVE_VARARGS_H*/

/* Some systems define this! */
#undef vfprintf

extern int _doprnt ();

int
vfprintf (file, format, ap)
     FILE *file;
     const char *format;
     va_list ap;
{
   return _doprnt (format, ap, file);
}
