/* list.c -- maintain lists of types with forward pointer fields
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

#include "list.h"

List *
list_new (userdata)
     sic_ptr userdata;
{
  List *new = XMALLOC (List, 1);

  new->next = NULL;
  new->userdata = userdata;

  return new;
}

List *
list_cons (head, tail)
     List *head;
     List *tail;
{
  head->next = tail;
  return head;
}

List *
list_tail (head)
     List *head;
{
  return head->next;
}

List *
list_nth (head, n)
     List *head;
     size_t n;
{
  for (;n > 1 && head; n--)
    head = head->next;

  return head;
}

size_t
list_length (head)
     List *head;
{
  size_t n;
  
  for (n = 0; head; ++n)
    head = head->next;

  return n;
}
