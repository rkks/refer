/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Factory.cpp
 * Desc    :    Platform dependant objects creator
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Factory.h"
#include "Lock.h"
#include "Semaphore.h"
#include "Thread.h"
#include "ListeningSocket.h"
#include "Assert.h"
#include "Types.h"
#ifdef _WIN32
# include "WinLock.h"
# include "WinSemaphore.h"
# ifdef CreateSemaphore
#  undef CreateSemaphore
# endif /* CreateSemaphore */
# include "WinThread.h"
# include "WinListeningSocket.h"
#elif defined(__FreeBSD__) or defined (linux) or defined (__APPLE__) /* UNIX */
# include "UnixLock.h"
# include "UnixSemaphore.h"
# include "UnixThread.h"
# include "UnixListeningSocket.h"
# include <unistd.h>
#endif /* _WIN32 */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Lock* Factory::CreateLock(void)
{
#ifdef _WIN32
  return new WinLock();
#elif defined(__FreeBSD__) or defined(linux) or defined (__APPLE__)
  return new UnixLock();
#else
  FASSERT(false, "Unknown platform");
  return NULL;
#endif /* _WIN32 */
}

Semaphore* Factory::CreateSemaphore(void)
{
#ifdef _WIN32
  return new WinSemaphore();
#elif defined(__FreeBSD__) or defined(linux) or defined (__APPLE__)
  return new UnixSemaphore();
#else
  FASSERT(false, "Unknown platform");
  return NULL;
#endif /* _WIN32 */
}

Thread* Factory::CreateThread(fThreadStartRoutine _entryPoint, void* pvData)
{
#ifdef _WIN32
  return new WinThread(_entryPoint, pvData);
#elif defined(__FreeBSD__) or defined(linux) or defined (__APPLE__)
  return new UnixThread(_entryPoint, pvData);
#else
  FASSERT(false, "Unknown platform");
  return NULL;
#endif /* _WIN32 */
}

ListeningSocket*  Factory::CreateListeningSocket(unsigned short port)
{
#ifdef _WIN32
  return new WinListeningSocket(port);
#elif defined(__FreeBSD__) or defined(linux) or defined (__APPLE__)
  return new UnixListeningSocket(port);
#else
  FASSERT(false, "Unknown platform");
  return NULL;
#endif /* _WIN32 */  
}

void  Factory::Sleep(unsigned milliseconds)
{
#ifdef _WIN32
  ::Sleep(milliseconds);  
#elif defined(__FreeBSD__) or defined(linux) or defined (__APPLE__)
  ::usleep(milliseconds * 10);
#else
  FASSERT(false, "Unknown platform");
#endif /* _WIN32 */
}
