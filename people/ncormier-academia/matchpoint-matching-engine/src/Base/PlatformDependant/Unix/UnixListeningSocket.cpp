/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UnixListeningSocket.cpp
 * Desc    :    Server implementation for unix
 * Author  :    nico
 *****************************************************************************/
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include "UnixListeningSocket.h"
#include "UnixSocket.h"
#include "Assert.h"
#include "MatchTask.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UnixListeningSocket::UnixListeningSocket(unsigned short port) : ListeningSocket(port)
{
  // Setup socket
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  int s = ::socket(AF_INET, SOCK_STREAM, 0);
  FASSERT(s != -1, "::socket() failed");
  // Set SO_REUSEADDR option to yes
  int yes = 1;
  int setOpt = ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  FASSERT(setOpt != -1, "::setsockopt() failed");
  int fcntlRet = ::fcntl(s, F_SETFL, O_NONBLOCK); /* Put socket in non-blocking mode */
  FASSERT(fcntlRet != -1, "::fcntl() failed");
  int bindRet = ::bind(s, (const struct sockaddr*) &sin, sizeof(sin));
  FASSERT(bindRet != -1, "::bind() failed");
  int listenRet = ::listen(s, SOMAXCONN);
  FASSERT(listenRet != -1, "::listen() failed");
  m_listenSock = s; /* Assign */
}

UnixListeningSocket::~UnixListeningSocket(void)
{
  (void) ::close(m_listenSock);
}

Socket* UnixListeningSocket::CheckForANewClient(void)
{
  // Check new connection
  struct sockaddr_in from;
  int sin_size = sizeof(struct sockaddr_in);
  int socket = ::accept(m_listenSock, (struct sockaddr*) &from, (socklen_t*) &sin_size);
  if (socket != -1)
  {
    // New client !
    UnixSocket* newClt = new UnixSocket(socket);
    return newClt;
  }
  TODO; /* FASSERT if socket has been closed */
  return NULL;
}
