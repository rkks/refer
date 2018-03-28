/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Entity.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __ENTITY_H__
# define __ENTITY_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "Types.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class Task;
class Thread;
class TaskFifo;
class CoreEntity;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
#ifdef EXIT_SUCCESS
# undef EXIT_SUCCESS
#endif /* EXIT_SUCCESS */
class Entity
{
  friend class CoreEntity; /* Core is a special entity */
  /***************************************************************************/
  /* Public methods                                                          */
  /***************************************************************************/
public:
                              Entity(unsigned id);
  virtual                    ~Entity(void);
  virtual const char*         Description(void) = 0;
  // Main thread context
  void                        AddTask(Task* newTask);         
  bool                        CancelTask(Task* taskToCancel);
  void                        Launch(void);
  /***************************************************************************/
  /* Protected types definition                                              */
  /***************************************************************************/
protected:
  enum _eEntryPointStatus
  {
    EXIT_SUCCESS = 0,
    EXIT_FAIL,
    EXIT_OUPS
  };
  typedef enum _eEntryPointStatus eEntryPointStatus;
  /***************************************************************************/
  /* Protected/Private methods                                               */
  /***************************************************************************/
protected:
  virtual eEntryPointStatus   entryPoint(void) = 0;
private:
  static void                 _entryPoint(Entity* self);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  TaskFifo*     m_fifo; /* Thread safe */
private:
  Thread*       m_thread;
  unsigned      m_id; /* Thread safe, should never change */
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __ENTITY_H__ */
