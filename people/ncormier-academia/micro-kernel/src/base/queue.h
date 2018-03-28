/*
** queue.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __BASE_QUEUE_H__
# define __BASE_QUEUE_H__


# include <kernel/common/kalloc.h> /* cf TODO */


/* Singly linked list helper
 *  usage: 
 *	SINGLY_LINKED_LIST(toto, int) my_int_list;
 *	SINGLY_LINKED_LIST_INIT(my_int_list);
 *	SINGLY_LINKED_LIST_ITER(toto) iter = SINGLY_LINKED_LIST_BEGIN(my_int_list);
 *	for (; iter != SINGLY_LINKED_LIST_EOF(iter); iter = SINGLY_LINKED_LIST_NEXT(iter))
 *	  iter->value = 1;
 *  TODO:
 *	@ check macro __KERNEL__ and switch between malloc and kalloc
 */
# define SINGLY_LINKED_LIST(name, type)					\
  struct name								\
  {									\
    struct name*	next;						\
    type		value;						\
  } *

# define SINGLY_LINKED_LIST_INIT(list)					\
  list = 0x0;

# define SINGLY_LINKED_LIST_BEGIN(list)					\
  (list)

# define SINGLY_LINKED_LIST_EOF(iter)					\
  ((!iter) || (!iter->next))

# define SINGLY_LINKED_LIST_ITER(name)					\
  struct name*

# define SINGLY_LINKED_LIST_NEXT(iter)					\
  iter->next

# define SINGLY_LINKED_LIST_VALUE(iter)					\
  iter->value

# define SINGLY_LINKED_LIST_EMPTY(list)					\
  SINGLY_LINKED_LIST_EOF(list)

# define SINGLY_LINKED_LIST_FIRST_VALUE(list)				\
  SINGLY_LINKED_LIST_EOF(list) ? (0x0)					\
			       : (SINGLY_LINKED_LIST_VALUE(list))
			       

# define SINGLY_LINKED_LIST_POP(list)					\
    if (!SINGLY_LINKED_LIST_EMPTY(list))				\
      list = SINGLY_LINKED_LIST_NEXT(list)

# define SINGLY_LINKED_LIST_ALLOC_ENTRY(name)				\
  ((struct name*) kalloc(sizeof(struct name)))

# define SINGLY_LINKED_LIST_ASSIGN_ENTRY(entry, value_to_assign)	\
  entry->value = value_to_assign

# define SINGLY_LINKED_LIST_INSERT(list, entry)				\
  entry->next = list;							\
  list = entry

# define SINGLY_LINKED_LIST_PUSH(name, list, value)			\
  {									\
    struct name* __entry = SINGLY_LINKED_LIST_ALLOC_ENTRY(name);	\
    SINGLY_LINKED_LIST_ASSIGN_ENTRY(__entry, value);			\
    SINGLY_LINKED_LIST_INSERT(list, __entry);				\
  }

# define SINGLY_LINKED_LIST_REMOVE(name, list, iter_seek)		\
  {									\
    struct name* __last = 0x0;						\
    SINGLY_LINKED_LIST_ITER(name) iter =				\
      SINGLY_LINKED_LIST_BEGIN(list);					\
    while (!SINGLY_LINKED_LIST_EOF(iter))				\
    {									\
      if (iter == iter_seek)						\
	{								\
	  if (!__last)							\
	    list = iter->next;						\
	  else								\
	    __last->next = iter->next;					\
	  break;							\
	}								\
      __last = iter;							\
      iter = SINGLY_LINKED_LIST_NEXT(iter);				\
    }									\
  }



#endif /* __BASE_QUEUE_H__ */
