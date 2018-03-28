/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Task.cpp
 * Desc    :    Task object implementation
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Task.h"
#include "Factory.h"
#include "Lock.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Task::Task(void)
{
  // Initialization
  m_referenceCount = 1; /* Auto retain at creation */
  m_status = PROCESSING;
  m_step = 0; /* Default step */
  m_lock = Factory::CreateLock();
  // Values set by the core entity
  m_route = (unsigned)-1;
  m_ownerID = (unsigned)-1;
}

Task::~Task(void)
{
  // Clean up
  if (m_lock)   delete m_lock;
}

Task*  Task::Retain(void)
{
  m_lock->Retain();
    m_referenceCount++;
  m_lock->Release();
  return this;
}

Task*  Task::Release(void)
{
  m_lock->Retain();
    m_referenceCount--;
  m_lock->Release();
  return this;
}

void  Task::ChangeStatus(eStatus newStatus)
{
  m_lock->Retain();
    m_status = newStatus;
  m_lock->Release();
}

eStatus Task::Status(void)
{
  eStatus status;
  m_lock->Retain();
    status = m_status;
  m_lock->Release();
  return status;
}
