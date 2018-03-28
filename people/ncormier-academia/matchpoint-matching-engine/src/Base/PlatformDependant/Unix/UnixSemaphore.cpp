/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UnixSemaphore.cpp
 * Desc    :    Semaphore implementation for unix
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UnixSemaphore.h"
#include "Assert.h"
#ifdef __APPLE__
# include <unistd.h>
# include <sys/stat.h>
#endif /* __APPLE__ */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UnixSemaphore::UnixSemaphore(void)
{
  // Initialize objects
#ifndef __APPLE__
  m_sem = &m_dummy;
  int ret = ::sem_init(m_sem, 0, 0);
  FASSERT(ret == 0, "::sem_init() failed");
#else
  char buf[32] = "XXXXXX";
  ::mktemp(buf);
  m_sem = ::sem_open(buf, O_CREAT|O_EXCL, S_IRWXU, 0);
  FASSERT(m_sem != (sem_t*)SEM_FAILED, "::sem_open() failed");  
#endif /* __APPLE__ */
}

UnixSemaphore::~UnixSemaphore(void)
{
  // Destroy objects
  (void) ::sem_destroy(m_sem);
}

bool  UnixSemaphore::TryDecrease(void)
{
  int ret = ::sem_trywait(m_sem);
  return ret == 0;
}

void  UnixSemaphore::Decrease(void)
{
  int ret = ::sem_wait(m_sem);
  FASSERT(ret == 0, "::sem_wait() failed");
}

void  UnixSemaphore::Increase(unsigned count /* = 1*/)
{
  for (unsigned i = 0; i < count; i++)
  {
    int ret = ::sem_post(m_sem);
    FASSERT(ret == 0, "::sem_post() failed");    
  }
}
