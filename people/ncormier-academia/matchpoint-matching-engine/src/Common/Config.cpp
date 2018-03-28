/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Config.cpp
 * Desc    :
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __CONFIG_CPP__
#define DEFAULT_PORT      4242
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Config.h"
#include "Factory.h"
#include "Lock.h"
#include "Interfaces.h"
#include "Assert.h"
#include "MString.h"
#include "ConfigAdapter.h"
#include "CommandLine.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Static variables                                                          */
/*****************************************************************************/
MutableConfig* ConfigHandler::s_mutableConfigInstance = NULL;
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 *  Config implementation
 */
Config::Config(void)
{
  // Objects creation
  m_lock = Factory::CreateLock();
  m_ports = new Ports();
  m_memcacheservers = new MemcacheServers();
  m_mapInterfaces = new MapInterfaces();
  // Default values
  m_ports->push_front(DEFAULT_PORT);
  /* TEMPORARY */
  m_bddType = new MString();
  m_bddHost = new MString();
  m_bddUser = new MString();
  m_bddPasswd = new MString();
  m_bddPort = new MString();
  m_bddDB = new MString();

}

Config::~Config(void)
{
  // Objects cleanup
  if (m_lock)           delete m_lock;
  if (m_ports)          delete m_ports;
  /* foreach delete servers */
  if (m_memcacheservers)          delete m_memcacheservers;
  /* Foreach map interface delete */
  if (m_mapInterfaces)  delete m_mapInterfaces;
  /* TEMPORARY */
  if (m_bddType)        delete m_bddType;
  if (m_bddHost)        delete m_bddHost; 
  if (m_bddUser)        delete m_bddUser;
  if (m_bddPasswd)      delete m_bddPasswd;
  if (m_bddPort)        delete m_bddPort;
  if (m_bddDB)          delete m_bddDB;
}

bool Config::PortAtIndex(unsigned index, unsigned short& port)
{
  bool ret = false;
  m_lock->Retain();
  {
    Ports::const_iterator iter;
    for (iter = m_ports->begin(); index && iter != m_ports->end();
          iter++, index--) ; /* Iterate to index */
    if (!index && iter != m_ports->end())
    {
      port = *iter;
      ret = true;
    }
  }
  m_lock->Release();
  return ret;
}

bool Config::MemcacheServerAtIndex(unsigned index, MString &host, unsigned short& port)
{
  bool ret = false;
  m_lock->Retain();
  {
    MemcacheServers::const_iterator iter;
    for (iter = m_memcacheservers->begin(); index && iter != m_memcacheservers->end();
          iter++, index--) ; /* Iterate to index */
    if (!index && iter != m_memcacheservers->end())
    {
      MemcacheServer *tmp = (MemcacheServer*)*iter;
      port = tmp->port;
      host = *tmp->host;
      ret = true;
    }
  }
  m_lock->Release();
  return ret;
}

unsigned int Config::GetCacheExpire(void)
{
  return m_cacheexpire;
}

const MapInterface& Config::MapInterfaceById(const MString& id)
{
  MutableMapInterface* ret = NULL;
  m_lock->Retain();
  {
      for (MapInterfaces::iterator iter = m_mapInterfaces->begin();
          iter != m_mapInterfaces->end(); iter++)
    {
      if ((*iter)->MapID() == id)
      {
        ret = *iter;
        break;
      }
    }
  }
  m_lock->Release();
  if (ret)
    return *ret;
  FASSERT(false, "Unknown MapInterface");
  return InvalidMapInterface::Singleton();
}

const MString&  Config::BDDType(void)
{
  return *m_bddType;
}

const MString&  Config::BDDHost(void)
{
  return *m_bddHost;
}

const MString&  Config::BDDUser(void)
{
  return *m_bddUser;
}

const MString&  Config::BDDPasswd(void)
{
  return *m_bddPasswd;
}

const MString&  Config::BDDPort(void)
{
  return *m_bddPort;
}

const MString&  Config::BDDDB(void)
{
  return *m_bddDB;
}

/*
 *  MutableConfig implementation
 */
MutableConfig::MutableConfig(void)
{
}

MutableConfig::~MutableConfig(void)
{
}

void  MutableConfig::AddPort(unsigned short port)
{
  for (Ports::const_iterator iter = m_ports->begin(); iter != m_ports->end(); iter++)
  {
    if (*iter == port)
      return;
  }
  m_lock->Retain();
    m_ports->push_back(port);
  m_lock->Release();
}

void  MutableConfig::RemovePort(unsigned short port)
{
  m_lock->Retain();
    m_ports->remove(port);
  m_lock->Release();
}

MutableMapInterface*  MutableConfig::AddMapInterface(const MString& id, const MString& tableName)
{
  MutableMapInterface* newMap = new MutableMapInterface(id, tableName);
  m_lock->Retain();
    m_mapInterfaces->push_back(newMap);
  m_lock->Release();
  return newMap;
}

void  MutableConfig::RemoveMapInterface(const MString& id)
{
  MutableMapInterface* toDelete = NULL;
  m_lock->Retain();
  {
      for (MapInterfaces::iterator iter = m_mapInterfaces->begin();
          iter != m_mapInterfaces->end(); iter++)
    {
      if ((*iter)->MapID() == id)
      {
        toDelete = *iter;
        break;
      }
    }
    FASSERT(toDelete != NULL, "Unknown interface");
    m_mapInterfaces->remove(toDelete);
  }
  m_lock->Release();
}

void  MutableConfig::SetBDDConfig(const MString& type, const MString& host, const MString& user, const MString& passwd, const MString& port, const MString& db)
{
  *m_bddType = type;
  *m_bddHost = host;
  *m_bddUser = user;
  *m_bddPasswd = passwd;
  *m_bddPort = port;
  *m_bddDB = db;
}

void  MutableConfig::AddMemcacheServer(const MString &host, const MString &port)
{
  for (MemcacheServers::const_iterator iter = m_memcacheservers->begin(); iter != m_memcacheservers->end(); iter++)
  {
    if (*(*iter)->host == host)
      return;
  }

  MemcacheServer *server = new MemcacheServer;

  server->host = new MString(host);
  
  int val = port.ToInt();
  FASSERT(val > 0 && val < 65535, "wrong tcp port number for memcache server");
  server->port = val;

  m_lock->Retain();
  m_memcacheservers->push_back(server);
  m_lock->Release();
}

void MutableConfig::SetCacheExpire(const MString &expire)
{
  int val = expire.ToInt();
  FASSERT(val >= 0, "expiration time for cache must be at least 0 second");
  m_cacheexpire = val;
}

/*
 *  MutableConfig implementation
 */
Config&  ConfigHandler::MainConfig(void)
{
  return MainMutableConfig();
}

MutableConfig& ConfigHandler::MainMutableConfig(void)
{
  if (!s_mutableConfigInstance)
    //s_mutableConfigInstance = new MutableConfig();
    s_mutableConfigInstance = ConfigAdapter::Create(CommandLineHandler::MainCommandLine().ConfigFilePath().CharArray()); /* Temporary, should be in IOEntity */
  return *s_mutableConfigInstance;
}
