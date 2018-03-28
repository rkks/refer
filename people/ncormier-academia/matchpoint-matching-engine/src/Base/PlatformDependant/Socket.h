/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Socket.h
 * Desc    :    Socket class declaration
 *              Only used by the network entity
 *              Temporary implementation: multi-thread read/write ?
 * Author  :    nico
 *****************************************************************************/

#ifndef __SOCKET_H__
# define __SOCKET_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
#if 0
class Lock;
#endif
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
* eSocketStatus
*/
enum _eSocketStatus
{
  SOCKET_ACTIVE = 0,
  SOCKET_CLOSED
};
typedef enum _eSocketStatus eSocketStatus;
/*
* Socket
*/
class Socket
{
public:
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                    Socket(void); /* Must use the factory to create it */
public:
  virtual          ~Socket(void);
  unsigned          Read(char* buf, unsigned nbrBytes, bool removeFromCache = true);
  unsigned          Read(char* buf, unsigned nbrBytes, int index, bool removeFromCache = true);
  void              Write(const char* buf, unsigned nbrBytes);
  void              ProcessRead(void); /* Should be called in another thread than network entity thread ? */
  void              ProcessWrite(void); /* Should be called in another thread than network entity thread ? */
  unsigned          countBytesInReadBuffer(void);
  unsigned          countBytesInWriteBuffer(void);
  eSocketStatus     Status(void);
protected:
  virtual void      systemRead(char* buf, unsigned nbrBytes, unsigned& readBytes) = 0; /* Non blocking call */
  virtual void      systemWrite(const char* buf, unsigned nbrBytes, unsigned& writtenBytes) = 0; /* Non blocking call */
  void              setStatus(eSocketStatus status);
private:
  void              concatBuffers(const char* inputBuf, unsigned nbrBytes, char*& buf, unsigned& bufSize, unsigned& bufAllocSize);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  eSocketStatus     m_status; /* Thread safe ? */
#if 0
  Lock*             m_lock;
#endif
  char*             m_readBuf;
  unsigned          m_readBufSize;
  unsigned          m_readBufAllocSize;
  char*             m_writeBuf;
  unsigned          m_writeBufSize;
  unsigned          m_writeBufAllocSize;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __SOCKET_H__ */
