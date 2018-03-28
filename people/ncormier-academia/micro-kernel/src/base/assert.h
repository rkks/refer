/*
** assert.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/

#ifndef __BASE_ASSERT_H__
# define __BASE_ASSERT_H__


# include <base/types.h>
# include <base/debug.h>


/* TODO:
 *	@ Check macro _KERNEL_ and switch to the good assertion function
 */
# define ASSERT(cond, msg)			\
  if (!(cond))					\
    {						\
      TRACE("");				\
      TRACE("KERNEL PANIC - ASSERT FAILED");	\
      TRACE(msg);				\
      __asm__("cli"); /* TODO !! HAL !! */	\
      while(1);					\
    }


#endif /* __BASE_ASSERT_H__ */
