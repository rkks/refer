/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    NetworkEntity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __NETWORK_ENTITY_CPP__
#define FOREACH_LISTENING_PORT                                          \
                                for (ListeningSockets::const_iterator   \
                                    iter = m_listeningSockets->begin(); \
                                    iter != m_listeningSockets->end(); iter++)
#define INLOOP_LISTENING_PORT   (*iter)
#define SEQUENCE_END            "\r\n.\r\n"
#define SEQUENCE_END_B          "\n.\n" /* TEMPORARY */
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <string.h>
#include "NetworkEntity.h"
#include "ListeningSocket.h"
#include "Socket.h"
#include "Factory.h"
#include "TaskFifo.h"
#include "MatchTask.h"
#include "Assert.h"
#include "CoreEntity.h"
#include "Config.h"
#include "NotificationTask.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
NetworkEntity::NetworkEntity(unsigned id) : Entity(id)
{
  // Objects creation
  Config& conf = ConfigHandler::MainConfig();
  m_listeningSockets = new ListeningSockets();
  for (unsigned short i = 0, port; conf.PortAtIndex(i, port); i++) /* Foreach ports */
    m_listeningSockets->push_back(Factory::CreateListeningSocket(port));
  FASSERT(m_listeningSockets->size() != 0, "Bad config, must have one or more listening port");
  m_readQueue = new SocketQueue();
  m_sleepQueue = new TaskQueue();
  m_writeQueue = new TaskQueue();
  Launch(); /* Launch entity ! */
}

NetworkEntity::~NetworkEntity(void)
{
  // Objects destruction
  if (m_listeningSockets)
  {
    FOREACH_LISTENING_PORT  delete INLOOP_LISTENING_PORT; /* Cleanup each listening port */
    delete m_listeningSockets;
  }
  if (m_readQueue)        delete m_readQueue;
  if (m_sleepQueue)       delete m_sleepQueue;
  if (m_writeQueue)       delete m_writeQueue;
}

const char* NetworkEntity::Description(void)
{
  return "NetworkEntity";
}

NetworkEntity::eEntryPointStatus  NetworkEntity::entryPoint(void)
{
  /*
   *  Temporary implementation
   *  Should add a sleep
   */
  while (true)
  {
    // Check for a new client
    FOREACH_LISTENING_PORT
    {
      Socket* newClt = INLOOP_LISTENING_PORT->CheckForANewClient();
      if (newClt) /* Add new socket to read queue */
        m_readQueue->push_back(newClt);
    }
    checkReadQueue(); /* Check read queue */
    checkIncomingTask(); /* Check for a new task */
    checkWriteQueue(); /* Check write queue */
    Factory::Sleep(5);
  }
  return EXIT_SUCCESS;
}

bool  NetworkEntity::requestIsComplete(Socket* sock)
{
  /*
  * TODO: multi-bytes string
  * TEMPORARY !!
  */
  {
    static const int epilogueLen = sizeof(SEQUENCE_END) - 1; /* -1 for null terminated string */
    if (sock->countBytesInReadBuffer() < epilogueLen)
      return false;
    char buf[sizeof(SEQUENCE_END)];
    (void) ::memset(buf, 0, sizeof(buf));
    (void) sock->Read(buf, sizeof(buf), -epilogueLen, false); /* Just check in buffer */
    if (!strncmp(SEQUENCE_END, buf, sizeof(SEQUENCE_END)))
      return true;
  }
  {
    static const int epilogueLen = sizeof(SEQUENCE_END_B) - 1; /* -1 for null terminated string */
    if (sock->countBytesInReadBuffer() < epilogueLen)
      return false;
    char buf[sizeof(SEQUENCE_END_B)];
    (void) ::memset(buf, 0, sizeof(buf));
    (void) sock->Read(buf, sizeof(buf), -epilogueLen, false); /* Just check in buffer */
    if (!strncmp(SEQUENCE_END_B, buf, sizeof(SEQUENCE_END_B)))
      return true;
  }
  return false;
}

void  NetworkEntity::checkReadQueue(void)
{
  bool readCompletedOrSockedClosed;
  do
  {
    readCompletedOrSockedClosed = false;
    for (SocketQueue::iterator iter = m_readQueue->begin();
      iter != m_readQueue->end(); iter++)
    {
      (*iter)->ProcessRead(); /* Try to read new data */
      if ((*iter)->Status() != SOCKET_ACTIVE) /* Socket has been close or an error has occurred */
      {
        readCompletedOrSockedClosed = true;
        delete *iter; /* Just delete socket */
        m_readQueue->remove(*iter); /* Remove from the read queue */
        break;
      }
      if (requestIsComplete(*iter))
      {
        readCompletedOrSockedClosed = true;
        // Create a new task and send it to the core
        MatchTask* newMtTsk = dynamic_cast<MatchTask*>(CORE_ALLOC_TASK());
        FASSERT(newMtTsk != NULL, "Bad task type");
        newMtTsk->m_clientSocket = *iter;
        newMtTsk->m_rawRequestLen = (*iter)->countBytesInReadBuffer();
        newMtTsk->m_rawRequest = new char[newMtTsk->m_rawRequestLen]; /* Allocate request buffer */
        (*iter)->Read(newMtTsk->m_rawRequest, newMtTsk->m_rawRequestLen); /* Transfert request from socket buffer to task buffer */
        m_sleepQueue->push_back(newMtTsk->Retain()); /* Add to the sleep queue */
        newMtTsk->Release(); /* Release task */
        CORE_TREAT_TASK(newMtTsk); /* Send new task to core */
        m_readQueue->remove(*iter); /* Remove from the read queue */
        break;
      }
    }
  }
  while (readCompletedOrSockedClosed);
}

void  NetworkEntity::checkIncomingTask(void)
{
  Task* tsk = m_fifo->TryGetATask(); 
  if (tsk)
  {
    NotificationTask* notifTsk = dynamic_cast<NotificationTask*>(tsk);
    if (notifTsk) /* Notification task */
    {
      /* TODO */
      CORE_TREAT_TASK(notifTsk); /* Notification handled release to core */
    }
    else
    {
      MatchTask* mtTsk = dynamic_cast<MatchTask*>(tsk);
      FASSERT(mtTsk != NULL, "Bad task type");
      m_sleepQueue->remove(mtTsk->Release());
      m_writeQueue->push_back(mtTsk->Retain());
      FASSERT(mtTsk->m_rawAnswer != NULL, "Null pointer m_rawAnswer");
      mtTsk->m_clientSocket->Write(mtTsk->m_rawAnswer, 
                                    (unsigned) ::strlen(mtTsk->m_rawAnswer)); /* Copy answer data to socket buffer */
    }
  }
}

void NetworkEntity::checkWriteQueue(void)
{
  bool writeCompletedOrSockedClosed;
  do
  {
    writeCompletedOrSockedClosed = false;
    for (TaskQueue::iterator iter = m_writeQueue->begin();
      iter != m_writeQueue->end(); iter++)
    {
      MatchTask* mtTsk = dynamic_cast<MatchTask*>(*iter);
      FASSERT(mtTsk != NULL, "Bad task type");
      mtTsk->m_clientSocket->ProcessWrite(); /* Try to write some data */
      if (mtTsk->m_clientSocket->Status() != SOCKET_ACTIVE) /* Socket has been close or an error has occurred */
      {
        writeCompletedOrSockedClosed = true;
        Task* tsk = *iter;
        m_writeQueue->remove(tsk->Release()); /* Remove from the write queue */
        tsk->ChangeStatus(PROCESS_FAILED); /* Change task's status */
        CORE_TREAT_TASK(tsk); /* Send it to core (core will delete socket) */
        break;
      }
      if (!mtTsk->m_clientSocket->countBytesInWriteBuffer()) /* Write buffer is empty, task is finished */
      {
        writeCompletedOrSockedClosed = true;
        Task* tsk = *iter;
        m_writeQueue->remove(tsk->Release()); /* Remove from the write queue */
        CORE_TREAT_TASK(tsk); /* Send it to core (core will delete socket) */
        break;
      }
    }
  }
  while (writeCompletedOrSockedClosed);
}
