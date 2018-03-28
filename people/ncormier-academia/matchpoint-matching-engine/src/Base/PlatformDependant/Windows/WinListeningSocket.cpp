/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    WinListeningSocket.cpp
 * Desc    :    Server implementation for windows
 * Author  :    nico
 *****************************************************************************/

#pragma comment(lib, "ws2_32.lib")
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <winsock2.h>
#include "WinListeningSocket.h"
#include "WinSocket.h"
#include "Assert.h"
#include "MatchTask.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
WinListeningSocket::WinListeningSocket(unsigned short port) : ListeningSocket(port)
{
  int nRet;
  WSADATA wsaData;
  nRet = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
  FASSERT(!nRet, "WSAStartup failed");
  m_listenSock = (unsigned int) ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
  FASSERT(m_listenSock != INVALID_SOCKET, "WSASocket() failed");
  // Set SO_REUSEADDR option to yes
  char yes = 1;
  int setOpt = ::setsockopt(m_listenSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  FASSERT(setOpt != -1, "::setsockopt() failed");
  // Pass socket in non blocking mode
  unsigned int nonblocking = 1;
  DWORD bytesReturned;
  nRet = ::WSAIoctl(m_listenSock, FIONBIO, &nonblocking,               
                    sizeof(nonblocking), NULL, 0, &bytesReturned, NULL, NULL);
  FASSERT(!nRet, "WSAIoctl() failed");
  // Bind socket
  sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = INADDR_ANY;
  local.sin_port = htons((u_short) port);
  nRet = ::bind(m_listenSock, (sockaddr*)&local, sizeof(local));
  FASSERT(!nRet, "bind() failed");
  nRet = ::listen(m_listenSock, SOMAXCONN);
  FASSERT(!nRet, "listen() failed");
}

WinListeningSocket::~WinListeningSocket(void)
{
  (void) closesocket(m_listenSock);
  (void) WSACleanup();
}

Socket* WinListeningSocket::CheckForANewClient(void)
{
  // Check new connection
  SOCKET socket = ::WSAAccept(m_listenSock, NULL, NULL, NULL, NULL);
  if (socket != INVALID_SOCKET)
  {
    // New client !
    WinSocket* newClt = new WinSocket(socket);
    return newClt;
  }
  TODO; /* FASSERT if socket has been closed */
  return NULL;
}
