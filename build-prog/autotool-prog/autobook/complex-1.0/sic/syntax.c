/* syntax.c -- create and administer syntax tables
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
#include "sic.h"
#include "ltdl.h"
#include "syntax.h"

struct syntaxnode {
  struct syntaxnode *next;
  lt_dlhandle module;
  SyntaxHandler *handler;
};

static struct syntaxnode *syntax_new (struct syntaxnode *head,
				      lt_dlhandle module,
				      SyntaxHandler *handler);
static int syntaxelt_address_compare (List *element, void *match);

int
syntax_install (Sic *sic, lt_dlhandle module, Syntax *table)
{
  Syntax *table_ptr;

  SIC_ASSERT (table);

  /* Allocate some room for the syntax tables. */
  if (!sic->syntax)
    sic->syntax = XCALLOC (SyntaxTable*, 256);

  for (table_ptr = table; table_ptr->handler; ++table_ptr)
    {
      char *ch_ptr = table_ptr->ch;
      
      /* You can set the syntax habdler for ASCII '\0' by using
	 "" in the syntax table. */
      if (*ch_ptr == 0)
	sic->syntax[0] = syntax_new (sic->syntax[0],
				     module, table_ptr->handler);

      /* Otherwise the handler is applied to each character in
	 the string of chars from the associated row. */
      for (; *ch_ptr; ++ch_ptr)
	sic->syntax[(int) *ch_ptr]
	  = syntax_new (sic->syntax[(int) *ch_ptr], module,
			table_ptr->handler);
    }

  return SIC_OKAY;
}
	
int
syntax_remove (Sic *sic, lt_dlhandle module, Syntax *table)
{
  /* FIXME: wwhere are the diagnostics? */
  int status = SIC_ERROR;
  Syntax *table_ptr;

  SIC_ASSERT (sic->syntax && table);
  
  /* Iterate over each entry in TABLE. */
  for (table_ptr = table; table_ptr->handler; ++table_ptr)
    {
      int i;

      /* Testing against each element of the installed syntax table. */
      for (i = 0; i < 256; ++i)
	{
	  List *stale = NULL;
	  
	  if (sic->syntax[i] != NULL)
	    {
	      stale =  list_remove ((List **) &sic->syntax[i],
				    (void **) module,
				    syntaxelt_address_compare);
	      if (stale)
		{
		  free (stale);

		  /* As soon as something that can be removed is
		     found, revoke the default error status.  */
		  status = SIC_OKAY;
		}
	    }
	}
    }
	      
  return status;
}

SyntaxHandler *
syntax_handler (Sic *sic, int ch)
{
  SIC_ASSERT (sic);
  
  return sic->syntax[ch] ? sic->syntax[ch]->handler : NULL;
}

static struct syntaxnode *
syntax_new (struct syntaxnode *head, lt_dlhandle module, SyntaxHandler *handler)
{
  struct syntaxnode *new = XCALLOC (struct syntaxnode, 1);

  new->next    = head;
  new->module  = module;
  new->handler = handler;

  return new;
}

static int
syntaxelt_address_compare (List *element, void *match)
{
  return (int) (((struct syntaxnode *) element)->module - match);
}
