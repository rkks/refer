/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    WinSocket.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#pragma comment(lib, "ws2_32.lib")
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <winsock2.h>
#include "WinSocket.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
WinSocket::WinSocket(SOCKET socket)
{
  m_socket = socket;
}

WinSocket::~WinSocket(void)
{
  (void) closesocket(m_socket);
}

/*
* systemRead
* ----------
* Non blocking call
*/
void  WinSocket::systemRead(char* buf, unsigned nbrBytes, unsigned& readBytes)
{
  WSABUF wsaBuf;
  wsaBuf.buf = (CHAR FAR*)buf;
  wsaBuf.len = nbrBytes;
  DWORD numberOfBytesRecvd;

#if 1
  numberOfBytesRecvd = ::recv(m_socket, buf, nbrBytes, 0);
  if (numberOfBytesRecvd == SOCKET_ERROR)
#else /* Ooups */
  if (::WSARecv(m_socket, &wsaBuf, 1, &numberOfBytesRecvd, 0, NULL, NULL) == SOCKET_ERROR)
#endif
  {
    int err = ::WSAGetLastError();
    switch (err)
    {
    case WSAEWOULDBLOCK: /* Nothing to do */
      break;
    default:
      FASSERT(false, "::WSARecv() failed");
    case WSAESHUTDOWN: /* Socket has been closed */
      setStatus(SOCKET_CLOSED);
      break;
    }
    readBytes = 0;
    return;
  }
  readBytes = numberOfBytesRecvd; 
}

/*
* systemWrite
* -----------
* Non blocking call
*/
void  WinSocket::systemWrite(const char* buf, unsigned nbrBytes, unsigned& writtenBytes)
{
  WSABUF wsaBuf;
  wsaBuf.buf = (CHAR FAR*)buf;
  wsaBuf.len = nbrBytes;
  DWORD numberOfBytesSent;

  int sendRet = ::WSASend(m_socket, &wsaBuf, 1, &numberOfBytesSent, 0, NULL, NULL);
  if (sendRet == SOCKET_ERROR)
  {
    int err = ::WSAGetLastError();
    switch (err)
    {
    case WSAESHUTDOWN: /* Socket has been closed */
      setStatus(SOCKET_CLOSED);
      break;
    case WSAEWOULDBLOCK: /* Nothing to do */
      break;
    default:
      FASSERT(false, "::WSASend() failed");
    }
    writtenBytes = 0;
    return;
  }
  writtenBytes = numberOfBytesSent;
}


