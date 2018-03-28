/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Thread.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Thread.h"
#include "Lock.h"
#include "Factory.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Thread::Thread(fThreadStartRoutine entryPoint, void* pvData)
{
  // Objects creation
  m_lock = Factory::CreateLock(); /* Create a platform dependant lock */
  // Initialization
  setState(TH_BEGIN);
  m_startRoutine = entryPoint;
  m_pvData = pvData;
}

Thread::~Thread(void)
{
  // Objects destruction
  if (m_lock) delete m_lock;
}

void  Thread::Start(void)
{
  setState(TH_RUN);
}

void  Thread::Suspend(void)
{
  setState(TH_SUSPEND);
}

void  Thread::Resume(void)
{
  setState(TH_RUN);
}

void  Thread::Terminate(void)
{
  setState(TH_END);
}

eThreadState Thread::State(void)
{
  m_lock->Retain();
    eThreadState ret = m_state;
  m_lock->Release();
  return ret;
}

void  Thread::setState(eThreadState newState)
{
  m_lock->Retain();
    m_state =  newState;
  m_lock->Release();
}
