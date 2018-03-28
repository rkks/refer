/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include <string.h>

#include <memcache.h>
#include <ctype.h>
#include <errno.h>

#include "Memcache.h"
#include "CRC32.h"
#include "Config.h"
#include "MString.h"

#ifndef _WIN32
static int32_t err_func(MCM_ERR_FUNC_ARGS)
{
  const struct memcache_ctxt *ctxt;
  struct memcache_err_ctxt *ectxt;

  MCM_ERR_INIT_CTXT(ctxt, ectxt);

  if (ectxt->errcode == MCM_ERR_SYS_READ)
    {
      ectxt->cont = 'y';
      errno = ECONNRESET;
    }

  if (ectxt->errcode == MCM_ERR_PROTO)
    ectxt->cont = 'y';

  return 0;
}
#endif

DNSPACE

Memcache::Memcache(void)
{
#ifndef _WIN32
  m_handle = mc_new();

  struct memcache_ctxt *ctxt;

  ctxt = mc_global_ctxt();
  ctxt->mcErr = err_func;

  Config& conf = ConfigHandler::MainConfig();

  MString host;
  unsigned short port;
  unsigned int expire;

  m_expire = conf.GetCacheExpire();

  bool added_one = false;

  for (unsigned short i = 0; conf.MemcacheServerAtIndex(i, host, port); i++)
    {
      AddServer(host.CharArray(), port);
      added_one = true;
    }

  if (added_one == false)
    /* no cache config */
    m_enabled = false;
  else
    m_enabled = true;
#endif
}

#if 0
Memcache::~Memcache(void)
{
#ifndef _WIN32
  mc_new(mc_handle);
#endif
}
#endif

Memcache &Memcache::MainHandler(void)
{
  static Memcache *singleton = NULL;

  if (singleton == NULL)
    singleton = new Memcache();

  return *singleton;
}

void Memcache::AddServer(const char *ip, unsigned short port)
{
#ifndef _WIN32
  char port_str[6];
  sprintf(port_str, "%i", port);

  mc_server_add(m_handle, ip, port_str);
#endif
}

int Memcache::Set(const char *key, void *value, int valuelen)
{
#ifndef _WIN32
  if (m_enabled == true)
    {
      CRC32 crc;
      unsigned int hash = crc.Compute(key, strlen(key));
      char hash_str[9];

      sprintf(hash_str, "%.8X\n", hash);
      return mc_add(m_handle, hash_str, 8, value, valuelen, m_expire, 0);
    }
#endif

  return 0;
}

void *Memcache::Get(const char *key, int *retlen)
{
#ifndef _WIN32
  if (m_enabled == true)
    {
      CRC32 crc;
      unsigned int hash = crc.Compute(key, strlen(key));
      char hash_str[9];

      sprintf(hash_str, "%.8X\n", hash);
      return mc_aget2(m_handle, hash_str, 8, (size_t*)retlen);
    }
#endif

  return NULL;
}
