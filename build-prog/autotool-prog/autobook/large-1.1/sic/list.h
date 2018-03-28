/* list.h -- maintain lists of types with forward pointer fields
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

#ifndef SIC_LIST_H
#define SIC_LIST_H 1

#include <sic/common.h>

BEGIN_C_DECLS

/* A generalised list.  This is deliberately transparent so that you
   can make the NEXT field of all your chained data structures first,
   and then cast them to `(List *)' so that they can be manipulated
   by this API.

   Alternatively, you can generate raw List elements using list_new(),
   and put the element data in the USERDATA field.  Either way you
   get to manage the memory involved by yourself.
*/
typedef struct list {
  struct list *next;		/* chain forward pointer*/
  sic_ptr userdata;		/* incase you want to use raw `List's */
} List;

extern List *list_new  PARAMS((sic_ptr userdata));
extern List *list_cons PARAMS((List *head, List *tail));
extern List *list_tail PARAMS((List *head));
extern List *list_nth  PARAMS((List *head, size_t n));
extern size_t list_length PARAMS((List *head));

END_C_DECLS

#endif /* !SIC_LIST_H */
