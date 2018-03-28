/* xmalloc.c -- memory management with out of memory reporting
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

#include "common.h"
#include "error.h"

sic_ptr
xcalloc (num, size)
     size_t num;
     size_t size;
{
#if HAVE_CALLOC
  sic_ptr new = calloc (num, size);
  if (!new)
    sic_fatal ("Memory exhausted");
#else
  sic_ptr new = xmalloc (num * size);
  bzero (new, num * size);
#endif

  return new;
}
     

sic_ptr
xmalloc (num)
     size_t num;
{
  sic_ptr new = malloc (num);
  if (!new)
    sic_fatal ("Memory exhausted");

  return new;
}

sic_ptr
xrealloc (p, num)
     sic_ptr p;
     size_t num;
{
  sic_ptr new;

  if (!p)
    return xmalloc (num);

  new = realloc (p, num);
  if (!new)
    sic_fatal ("Memory Exhausted");

  return new;
}
