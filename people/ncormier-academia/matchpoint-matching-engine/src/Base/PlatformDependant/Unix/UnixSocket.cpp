/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UnixSocket.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>
#include "UnixSocket.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UnixSocket::UnixSocket(int socket)
{
  m_socket = socket;
}

UnixSocket::~UnixSocket(void)
{
  (void) ::close(m_socket);
}

/*
* systemRead
* ----------
* Non blocking call
*/
void  UnixSocket::systemRead(char* buf, unsigned nbrBytes, unsigned& readBytes)
{
  unsigned readRet = ::read(m_socket, buf, nbrBytes);
  if (readRet == -1)
  {
    switch (errno)
    { 
    case EAGAIN: /* Nothing to do */
      break;
    default:
      FASSERT(false, "::read() failed");
    case ECONNRESET: /* Socket has been closed */
      setStatus(SOCKET_CLOSED);
      break;
    }
    readBytes = 0;
    return;
  }
  readBytes = readRet;
}

/*
* systemWrite
* -----------
* Non blocking call
*/
void  UnixSocket::systemWrite(const char* buf, unsigned nbrBytes, unsigned& writtenBytes)
{
  unsigned writeRet = ::write(m_socket, buf, nbrBytes);
  if (writeRet == -1)
  {
    switch (errno)
    { 
    case EAGAIN: /* Nothing to do */
      break;
    default:
      FASSERT(false, "::read() failed");
    case ECONNRESET: /* Socket has been closed */
      setStatus(SOCKET_CLOSED);
      break;
    }
  }
  writtenBytes = writeRet;
}


