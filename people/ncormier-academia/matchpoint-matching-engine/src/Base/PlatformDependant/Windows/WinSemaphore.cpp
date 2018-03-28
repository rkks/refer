/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    WinSemaphore.cpp
 * Desc    :    Semaphore implementation for windows
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "WinSemaphore.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
WinSemaphore::WinSemaphore(void)
{
  // Initialize objects
  m_sem = ::CreateSemaphore(NULL, 0, 255, NULL);
  FASSERT(m_sem ? true : false, "CreateSemaphore failed");
}

WinSemaphore::~WinSemaphore(void)
{
  // Destroy objects
  (void) ::CloseHandle(m_sem);
}

bool  WinSemaphore::TryDecrease(void)
{
  DWORD waitRet = ::WaitForSingleObject(m_sem, 0L);
  return waitRet == WAIT_OBJECT_0;
}

void  WinSemaphore::Decrease(void)
{
  DWORD waitRet = ::WaitForSingleObject(m_sem, INFINITE);
  FASSERT(waitRet == WAIT_OBJECT_0, "WaitForSingleObject failed");
}

void  WinSemaphore::Increase(unsigned count /* = 1*/)
{
  LONG lastCount;
  bool succeeded =::ReleaseSemaphore(m_sem, (LONG) count, &lastCount) ? true : false;
  FASSERT(succeeded, "::ReleaseSemaphore() failed");
}
