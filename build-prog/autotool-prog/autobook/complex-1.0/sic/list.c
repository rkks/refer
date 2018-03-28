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
list_new (void *userdata)
{
  List *new = XMALLOC (List, 1);

  new->next = NULL;
  new->userdata = userdata;

  return new;
}

/* Call CMP repeatedly with SEARCH and each element of *PHEAD, until
   CMP returns `0', or the list is exhausted.  If a match is found
   the matching element is removed from *PHEAD, and returned. */
List *
list_remove (List **phead, void *match, ListCmp *cmp)
{
  List *stale = NULL;
  
  if (!phead || !*phead || !match)
    return NULL;

  /* Does the head of the passed list match? */
  if ((*cmp) (*phead, match) == 0)
    {
      stale = *phead;
      *phead = stale->next;
    }
  /* what about the rest of the elements? */
  else
    {
      List *head;
      for (head = *phead; head->next; head = head->next)
	if ((*cmp) (head->next, match) == 0)
	  {
	    stale = head->next;
	    head->next = stale->next;
	  }
    }

  return stale;
}

List *
list_cons (List *head, List *tail)
{
  head->next = tail;
  return head;
}

List *
list_tail (List *head)
{
  return head->next;
}

List *
list_nth (List *head, size_t n)
{
  for (;n > 1 && head; n--)
    head = head->next;

  return head;
}

size_t
list_length (List *head)
{
  size_t n;
  
  for (n = 0; head; ++n)
    head = head->next;

  return n;
}

int
list_foreach (List *head, ListForeach *foreach, void *userdata)
{
  for (; head; head = head->next)
    if ((*foreach) (head, userdata) < 0)
      return -1;

  return 0;
}
