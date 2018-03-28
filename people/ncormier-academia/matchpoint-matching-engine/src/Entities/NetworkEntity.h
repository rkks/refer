/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    NetworkEntity.h
 * Desc    :    Temporary implementation: multi-thread read/write ?
 *                                        big buffer copy,
 *              we generate the complete answer and after generation we send it
 *              sleepQueue useless ?
 * Author  :    nico
 *****************************************************************************/

#ifndef __NETWORK_ENTITY_H__
# define __NETWORK_ENTITY_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#define __NETWORK_ENTITY_CPP__
# ifdef __NETWORK_ENTITY_CPP__
#  include <list>
# endif /* __NETWORK_ENTITY_CPP__ */
# include "Entity.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class ListeningSocket;
class Socket;
class Task;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class NetworkEntity : public Entity
{
  /***************************************************************************/
  /* Public methods                                                          */
  /***************************************************************************/
public:
                          NetworkEntity(unsigned id);
  virtual                ~NetworkEntity(void);
  const char*             Description(void);
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
private:
# ifdef __NETWORK_ENTITY_CPP__
  typedef std::list<Socket*>            SocketQueue;
  typedef std::list<Task*>              TaskQueue;
  typedef std::list<ListeningSocket*>   ListeningSockets;
#else
  typedef void SocketQueue, TaskQueue, ListeningSockets;
#endif /* __NETWORK_ENTITY_CPP__ */
  /***************************************************************************/
  /* Private methods                                                         */
  /***************************************************************************/
private:
  eEntryPointStatus       entryPoint(void);
  bool                    requestIsComplete(Socket* sock);
  void                    checkReadQueue(void);
  void                    checkIncomingTask(void);
  void                    checkWriteQueue(void);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  ListeningSockets*       m_listeningSockets;
  SocketQueue*            m_readQueue;
  TaskQueue*              m_sleepQueue;
  TaskQueue*              m_writeQueue;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __NETWORK_ENTITY_H__ */
