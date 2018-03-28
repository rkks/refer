/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Entity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Entity.h"
#include "Factory.h"
#include "TaskFifo.h"
#include "Task.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Entity::Entity(unsigned id)
{
  // Objects creation
  m_thread = Factory::CreateThread((fThreadStartRoutine)_entryPoint, (void*)this);
  m_fifo = new TaskFifo();
  // Initialization
  m_id = id;
}

Entity::~Entity(void)
{
  if (m_thread) delete m_thread;
  if (m_fifo)   delete m_fifo;
}

/*
* AddTask
* -------
* Add a task to the thread fifo
*/
void Entity::AddTask(Task* newTask)
{
  m_fifo->AddATask(newTask);
}

bool  Entity::CancelTask(Task* taskToCancel)
{
  TODO
  return false;
}

void  Entity::Launch(void)
{
  // Launch entity
  m_thread->Start();
}

/*
* entryPoint
* -----------
* Private method
* Thread entry point
*/
void Entity::_entryPoint(Entity *self)
{
  
  // Switch self->entryPoint for log ?
  // Sent to core ?
  // ...
  TODO;
  (void) self->entryPoint();
}
