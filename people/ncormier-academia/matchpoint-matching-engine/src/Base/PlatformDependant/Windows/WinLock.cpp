/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    WinLock.cpp
 * Desc    :    Lock implementation for windows
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "WinLock.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
WinLock::WinLock(void)
{
  // Initialize objects
  ::InitializeCriticalSection(&m_crit); 
}

WinLock::~WinLock(void)
{
  // Destroy objects
  ::DeleteCriticalSection(&m_crit);
}

bool  WinLock::TryRetain(void)
{
  return ::TryEnterCriticalSection(&m_crit) ? true : false;
}

void  WinLock::Retain(void)
{
  ::EnterCriticalSection(&m_crit);
}

void  WinLock::Release(void)
{
  ::LeaveCriticalSection(&m_crit);  
}
