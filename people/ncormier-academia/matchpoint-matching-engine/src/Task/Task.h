/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Task.h
 * Desc    :    Task object declartion
 *              A task instance can't be delete directly, a system of refcount
 *              has been implanted. With this system, an entity can keep a task
 *              without being the owner.
 *
 *              WARNING: THE CONTENTS OF THE MESSAGE MUST BE THREAD SAFE IF A 
 *              TASK IS USED IN MORE THAN ONE THREAD. THREAD COUNT JUST 
 *              PREVENTS TASK TO BE DELETED WHILE IT IS USED IN ANOTHER THREAD.
 *
 *              Count reference explanation : at creation task have a refcount
 *              equal to one. When you store a task object in a var you have to
 *              call the Retain task method. In opposite before the var
 *              destruction you have to call Release.
 *              The core entity handles task delation, when treating a task
 *              the core checks the refcount, if the recount is equal to zero
 *              and task have no recipient -> deletion. Otherwise add the task
 *              in the core check list for a futher deletion.
 * Author  :    nico
 *****************************************************************************/

#ifndef __TASK_H__
# define __TASK_H__

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
class CoreEntity;
class Lock;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class Task
{
  friend class CoreEntity; /* CoreEntity handles : - task deletion
                                                   - task deliveries         */
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                Task(void);
  virtual      ~Task(void); /* Reference count only check by the core entity */
public:
  Task*         Retain(void);
  Task*         Release(void);
  void          ChangeStatus(eStatus newStatus);
  eStatus       Status(void);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  Lock*         m_lock;
  unsigned      m_referenceCount; /* Access with m_lock */
  eStatus       m_status; /* Access with m_lock */
  unsigned      m_route; /* Current route used */
  unsigned      m_step; /* Current step in processing
                           thread safe only used by CoreEntity */
  unsigned      m_ownerID; /* Thread safe only used by CoreEntity */
  
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __TASK_H__ */
