/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _MEMCACHE_H_
# define _MEMCACHE_H_

# include "DefaultNamespace.h"

struct memcache;

DNSPACE_OPEN

class Memcache
{
 public:
#if 0
  ~Memcache(void);
#endif
  static Memcache &MainHandler(void);
  void AddServer(const char *ip, unsigned short port);

  int Set(const char *key, void *value, int valuelen);
  void *Get(const char *key, int *retlen);

 private:
  struct memcache *m_handle;
  unsigned int m_expire;
  bool m_enabled;

  Memcache(void);
};

DNSPACE_CLOSE
#endif /* !_MEMCACHE_H_ */
