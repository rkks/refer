/*
** prim_types.c for  in /home/nico/lang/c/ftrace/src/api
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Thu Mar 23 17:25:03 2006 nicolas
** Last update Wed Apr 26 23:25:32 2006 nicolas cormier
*/

#include <stdlib.h>

#include "api/includes/prototypes/prim_types.h"


list_t*	list_add(list_t* list, void* value)
{
  list_t*	new = NULL;


  new = malloc(sizeof(list_t));

  if (new == NULL)
    return (NULL);
    
  new->value = value;

  if (list)
    {
      new->prev = list->prev;
      new->next = list;
      list->prev->next = new;
      list->prev = new;
    }
  else
    {
      new->prev = new;
      new->next = new;      
    }
  return (new);
}

list_t*	list_del(list_t* list, void* value)
{
  list_t*	del = NULL;
  list_t*	ret = NULL;

  if (list == NULL)
    return (NULL);

  del = list;
  while (del->value != value)
    {
      if (del == list->prev)
	break;
      del = del->next;
    }

  if (del->prev == del && del->next == del)
    {
      free(del);
      return (NULL);
    }

  del->prev->next = del->next;
  del->next->prev = del->prev;

  ret = del->next;

  free(del);

  return (ret);
}

void	list_free(list_t* list)
{
  for(; list; )
    list = list_del(list, list->value);
}
