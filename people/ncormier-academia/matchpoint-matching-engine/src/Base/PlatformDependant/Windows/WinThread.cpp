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
#include "WinThread.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
WinThread::WinThread(fThreadStartRoutine entryPoint, void* pvData) : Thread(entryPoint, pvData)
{
  // Initialize objects
  DWORD threadID;
  m_thread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) m_startRoutine,
                            (LPVOID) pvData, CREATE_SUSPENDED, &threadID);
  FASSERT(m_thread ? true : false, "CreateThread failed");
}

WinThread::~WinThread(void)
{
  // Destroy objects
  (void) ::TerminateThread(m_thread, EXIT_SUCCESS);
  (void) ::WaitForSingleObject(m_thread, INFINITE);
}

void  WinThread::Start(void)
{
  bool succeeded = ::ResumeThread(m_thread) ? true : false;
  FASSERT(succeeded, "::ResumeThread() failed");
  Thread::Start();
}

void  WinThread::Suspend(void)
{
  bool succeeded = ::SuspendThread(m_thread) ? true : false;
  FASSERT(succeeded, "::SuspendThread() failed");
  Thread::Suspend();
}

void  WinThread::Resume(void)
{
  bool succeeded = ::ResumeThread(m_thread) ? true : false;
  FASSERT(succeeded, "::ResumeThread() failed");
  Thread::Resume();
}

void  WinThread::Terminate(void)
{
  bool succeeded = ::TerminateThread(m_thread, EXIT_SUCCESS) ? true : false;
  FASSERT(succeeded, "::TerminateThread() failed");
  Thread::Terminate();
}
