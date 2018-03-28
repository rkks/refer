/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UnixThread.cpp
 * Desc    :    Thread implementation for unix
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UnixThread.h"
#include "Lock.h"
#include "Factory.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UnixThread::UnixThread(fThreadStartRoutine entryPoint, void* pvData) : Thread(entryPoint, pvData)
{
  // Lock creation
  m_lock = Factory::CreateLock();
  m_lock->Retain(); /* Lock */
  int ret = ::pthread_create(&m_thread, NULL, (void*(*)(void*))creationRoutine, (void *)this);
  FASSERT(ret == 0, "pthread_create() failed");
}

UnixThread::~UnixThread(void)
{
  (void) ::pthread_cancel(m_thread);
}

void  UnixThread::Start(void)
{
  FASSERT(State() == TH_BEGIN, "Start() has already been called");
  m_lock->Release(); /* Cf startRoutine */
  Thread::Start();
}

void  UnixThread::Suspend(void)
{
  FASSERT(false, "Not implemented yet");
  // Utiliser les signaux pthread
  // pthread_kill()
  TODO;
  Thread::Suspend();
}

void  UnixThread::Resume(void)
{
  FASSERT(false, "Not implemented yet");
  // Utiliser les signaux pthread
  // pthread_kill()
  TODO;
  Thread::Resume();
}

void  UnixThread::Terminate(void)
{
  int ret = ::pthread_cancel(m_thread);
  FASSERT(ret == 0, "pthread_cancel() failed");
  Thread::Terminate();
}

void* UnixThread::creationRoutine(UnixThread* self)
{
  self->m_lock->Retain(); /* Wait for start() */
  (void) self->m_startRoutine(self->m_pvData); /* Start thread */
  return NULL;
}
