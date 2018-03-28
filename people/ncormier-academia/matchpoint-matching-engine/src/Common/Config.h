/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Config.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __CONFIG_H__
# define __CONFIG_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifdef __CONFIG_CPP__
#  include <list>
# endif /* __CONFIG_CPP__ */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class Lock;
class IOEntity;
class MapInterface;
class MutableMapInterface;
class MString;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class Config
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                        Config(void);
  virtual              ~Config(void);
public:
  bool                  PortAtIndex(unsigned index, unsigned short& port);
  bool                  MemcacheServerAtIndex(unsigned index, MString &host, unsigned short& port);
  unsigned int          GetCacheExpire(void);
  const MapInterface&   MapInterfaceById(const MString& id);
  /* TEMPORARY */
  const MString&        BDDType(void);
  const MString&        BDDHost(void);
  const MString&        BDDUser(void);
  const MString&        BDDPasswd(void);
  const MString&        BDDPort(void);
  const MString&        BDDDB(void);
  /***************************************************************************/
  /* Protected types definition                                              */
  /***************************************************************************/
protected:
# ifdef __CONFIG_CPP__
  typedef std::list<unsigned short>       Ports;
  typedef std::list<MutableMapInterface*> MapInterfaces;
  typedef struct
  {
    MString *host;
    unsigned short port;
  } MemcacheServer;
  typedef std::list<MemcacheServer*> MemcacheServers;
# else
  typedef void Ports, MapInterfaces, MemcacheServers;
# endif /* __CONFIG_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  Lock*             m_lock; /* Protect properties */
  Ports*            m_ports;
  MemcacheServers*  m_memcacheservers;
  unsigned int      m_cacheexpire;
  MapInterfaces*    m_mapInterfaces;
  /* TEMPORARY */
  MString*          m_bddType;
  MString*          m_bddHost;
  MString*          m_bddUser;
  MString*          m_bddPasswd;
  MString*          m_bddPort;
  MString*          m_bddDB;
};
class MutableConfig : public Config
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                        MutableConfig(void);
  virtual              ~MutableConfig(void);
  void                  AddPort(unsigned short port);
  void                  RemovePort(unsigned short port);
  MutableMapInterface*  AddMapInterface(const MString& id, const MString& tableName); /* TODO: ref plutot que pointeur */
  void                  RemoveMapInterface(const MString& id);
  void                  SetBDDConfig(const MString& type, const MString& host, const MString& user, 
                                     const MString& passwd, const MString& port, const MString& db);
  void                  AddMemcacheServer(const MString &host, const MString &port);
  void                  SetCacheExpire(const MString &expire);
};
class ConfigHandler
{
  friend class IOEntity; /* Only IOEntity can get mutable config object */
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  static Config&        MainConfig(void);
private:
  static MutableConfig& MainMutableConfig(void);
  /***************************************************************************/
  /* Private static variables                                                */
  /***************************************************************************/
private:
  static MutableConfig* s_mutableConfigInstance;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __CONFIG_H__ */
