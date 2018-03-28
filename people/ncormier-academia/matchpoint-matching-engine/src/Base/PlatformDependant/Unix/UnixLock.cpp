/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UnixLock.cpp
 * Desc    :    Lock implementation for unix
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UnixLock.h"
#include "Assert.h"
#include "Types.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UnixLock::UnixLock(void)
{
  // Initialize objects
  int ret = ::pthread_mutex_init(&m_lock, NULL);
  FASSERT(ret == 0, "::pthread_mutex_init() failed");
}

UnixLock::~UnixLock(void)
{
  // Destroy objects
  (void) ::pthread_mutex_destroy(&m_lock);
}

bool  UnixLock::TryRetain(void)
{
  int ret = ::pthread_mutex_trylock(&m_lock);
  FASSERT(ret == 0, "::pthread_mutex_trylock() failed");
}

void  UnixLock::Retain(void)
{
  int ret = ::pthread_mutex_lock(&m_lock);
  FASSERT(ret == 0, "::pthread_mutex_lock() failed");
}

void  UnixLock::Release(void)
{
  int ret = ::pthread_mutex_unlock(&m_lock);
  FASSERT(ret == 0, "::pthread_mutex_unlock() failed");
}
