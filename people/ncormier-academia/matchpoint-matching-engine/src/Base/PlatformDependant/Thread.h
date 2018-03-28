/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Thread.h
 * Desc    :    
 * Author  :    nico
 ******************************************************************************/

#ifndef __THREAD_H__
# define __THREAD_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class Lock;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
* eThreadState
*/
enum _eThreadState
{
  TH_BEGIN,   /* Wait for running */
  TH_RUN,     /* Thread is running */
  TH_SUSPEND, /* Thread is suspended */
  TH_END,     /* Thread is dead */
};
typedef _eThreadState eThreadState;
/*
* fThreadStartRoutine
*/
typedef void(*fThreadStartRoutine)(void* self);
/*
* Thread
*/
class Thread
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:          // Must use the factory to create a thread
                    Thread(fThreadStartRoutine entryPoint, void* pvData);
public:
  virtual          ~Thread(void);
  virtual void      Start(void);
  virtual void      Suspend(void);
  virtual void      Resume(void);
  virtual void      Terminate(void);
  eThreadState      State(void);
private:
  void              setState(eThreadState newState);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  fThreadStartRoutine   m_startRoutine; /* Thread entry point */
  void*                 m_pvData;
private:
  eThreadState          m_state; /* MT usage, use m_lock to access */
  Lock*                 m_lock;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __THREAD_H__ */
