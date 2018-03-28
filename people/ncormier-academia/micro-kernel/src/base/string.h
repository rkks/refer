/*
** string.h
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
*/


#ifndef __BASE_STRING_H__
# define __BASE_STRING_H__


#include <base/types.h>


static inline void*	memset(void *b, int c, size_t len)
{
  for (char* ptr = (char*)b; len; len--) *ptr++ = (char)c; 
  return b;
}

static inline void*	memcpy(void *dst, const void *src, size_t len)
{
  const char* csrc = (const char*)src;
  for (char* cdst = (char*)dst; len; len--) *cdst++ = *csrc++;
  return dst;
}


#endif /* __BASE_STRING_H__ */
