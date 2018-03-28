/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    TaskFifo.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "TaskFifo.h"
#include "Semaphore.h"
#include "Task.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
TaskFifo::TaskFifo(void)
{
  // Objects creation
  m_sem = Factory::CreateSemaphore();
  m_fifo = new Fifo<Task*>();
}

TaskFifo::~TaskFifo(void)
{
  if (m_sem)    delete m_sem;
  if (m_fifo)
  {
    Task* tsk;
    while (m_fifo->Get(tsk)) /* Release each task */
      if (tsk) tsk->Release();
    delete m_fifo;
  }
}

Task* TaskFifo::GetATask(void)
{
  // Wait for a task
  m_sem->Decrease();
  Task* tsk = NULL;
  bool getRet = m_fifo->Get(tsk);
  FASSERT(getRet && tsk, "Fifo::Get failed");
  if (tsk) tsk->Release();
  return tsk;
}

Task* TaskFifo::TryGetATask(void)
{
  Task* tsk = NULL;
  if (m_sem->TryDecrease())
  {
    bool getRet = m_fifo->Get(tsk);
    FASSERT(getRet, "Fifo::Get failed");
    if (tsk) tsk->Release();
  }
  return tsk;
}

void  TaskFifo::AddATask(Task* task)
{
  task->Retain();
  m_fifo->Add(task);
  m_sem->Increase();
}

unsigned  TaskFifo::TasksCount(void)
{
  return m_fifo->Count();
}
