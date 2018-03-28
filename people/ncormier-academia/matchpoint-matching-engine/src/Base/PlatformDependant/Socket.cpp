/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Socket.cpp
 * Desc    :    TODO, multi thead implementation ?
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <string.h>
#include "Socket.h"
#if 0
# include "Lock.h"
# include "Factory.h"
#endif
#include "Assert.h"
#include "Types.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Socket::Socket(void)
{
  // Objects creation
#if 0
  m_lock = Factory::CreateLock();
#endif
  // Objects initialization
  m_readBuf = NULL;
  m_readBufSize = 0;
  m_writeBuf = NULL;
  m_writeBufSize = 0;
  m_status = SOCKET_ACTIVE;
  m_readBufAllocSize = 0;
  m_writeBufAllocSize = 0;
}

Socket::~Socket(void)
{
#if 0
  if (m_lock)     delete m_lock;
#endif
  if (m_readBuf)  delete[] m_readBuf;
  if (m_writeBuf) delete[] m_writeBuf;
}

unsigned  Socket::Read(char* buf, unsigned nbrBytes, bool removeFromCache /* = true*/)
{
  return Read(buf, nbrBytes, 0, removeFromCache);
}

/*
* Read
* ----
* WARNING: nbrBytes must be a 32bits type
*/
unsigned  Socket::Read(char* buf, unsigned nbrBytes, int index, bool removeFromCache /* = true*/)
{
#if 0
  m_lock->Retain();
  {
#endif
  const char* readBufEnd = m_readBuf + m_readBufSize;
  char* src = m_readBuf + index;
  if (index < 0) /* Negative index, start at the buffer end */
  {
    src += m_readBufSize;
    FASSERT(src >= m_readBuf && src < readBufEnd, "Socket::Read() invalid index"); /* Not enough bytes in read buffer */
  }
  if (src + nbrBytes >= readBufEnd) /* Too many requested bytes to read */
  {
    int requestedNbrBytes = (int)(readBufEnd - src);
    FASSERT(requestedNbrBytes >= 0, "Socket::Read() invalid index");
    nbrBytes = requestedNbrBytes;
  }
  (void) ::memcpy(buf, src, nbrBytes); /* Copy data to user buffer */
  if (removeFromCache) /* Remove readed data in internal buffer */
  {
    (void) ::memcpy(src, src + nbrBytes, readBufEnd - (src + nbrBytes));
    m_readBufSize -= nbrBytes;
  }
#if 0
  }
  m_lock->Release();
#endif
  return nbrBytes;
}

void  Socket::Write(const char* buf, unsigned nbrBytes)
{
#if 0
  m_lock->Retain();
#endif
  concatBuffers(buf, nbrBytes, m_writeBuf, m_writeBufSize, m_writeBufAllocSize);  
#if 0
  m_lock->Release();
#endif
}

void  Socket::ProcessRead(void)
{
  char      buf[1024];
  unsigned  readBytes;

  systemRead(buf, sizeof(buf), readBytes);
#if 0
  m_lock->Retain();
#endif
  concatBuffers(buf, readBytes, m_readBuf, m_readBufSize, m_readBufAllocSize);  
#if 0
  m_lock->Release();
#endif
}

void  Socket::ProcessWrite(void)
{
#if 0
  m_lock->Retain();
  {
#endif
  unsigned  writtenBytes;

  systemWrite(m_writeBuf, m_writeBufSize, writtenBytes);
  (void) ::memcpy(m_writeBuf, m_writeBuf + writtenBytes, m_writeBufSize - writtenBytes);
  m_writeBufSize -= writtenBytes;
#if 0
  }
  m_lock->Release();
#endif
}

unsigned  Socket::countBytesInReadBuffer(void)
{
#if 0
  m_lock->Retain();
#endif
  unsigned ret = m_readBufSize;
#if 0
  m_lock->Release();
#endif
  return ret;
}

unsigned  Socket::countBytesInWriteBuffer(void)
{
#if 0
  m_lock->Retain();
#endif
  unsigned ret = m_writeBufSize;
#if 0
  m_lock->Release();
#endif
  return ret;
}

eSocketStatus Socket::Status(void)
{
  // Thread safe ?
  return m_status;
}

void  Socket::setStatus(eSocketStatus status)
{
  // Thread safe ?
  m_status = status;  
}

void  Socket::concatBuffers(const char* inputBuf, unsigned nbrBytes, char*& buf, unsigned& bufSize, unsigned& bufAllocSize)
{
  if (!nbrBytes) /* Nothing to concat */
    return;
  // Increase buffer size
  if (bufAllocSize - bufSize < nbrBytes) /* Need to resize buffer */
  {
    while (bufAllocSize - bufSize < nbrBytes)
      bufAllocSize += 1024;
    char* newBuf = new char[bufAllocSize];
    (void) ::memcpy(newBuf, buf, bufSize);
    if (buf) delete[] buf;
    buf = newBuf;
  }
  // Copy data
  (void) ::memcpy(buf + bufSize, inputBuf, nbrBytes);
  bufSize += nbrBytes;
}
