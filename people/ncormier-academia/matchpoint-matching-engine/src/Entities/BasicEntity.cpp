/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    BasicEntity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "CoreEntity.h"
#include "TaskFifo.h"
#include "Task.h"
#include "BasicEntity.h"
#include "MatchTask.h"
#include "NotificationTask.h"
#include "Assert.h"
#include "Error.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
BasicEntity::BasicEntity(unsigned id) : Entity(id)
{
  Launch(); /* Launch entity ! */
}

BasicEntity::~BasicEntity(void)
{
}

BasicEntity::eEntryPointStatus  BasicEntity::entryPoint(void)
{
  while (true)
  {
    // Try to get a new task
    Task* tsk = m_fifo->GetATask();
    tsk->Retain(); /* Retain task */
    NotificationTask* notifTsk = dynamic_cast<NotificationTask*>(tsk);
    if (notifTsk) /* Notification task */
    {
      switch (notifTsk->Notification())
      {
      case NotificationTask::CONFIG_CHANGED:
        onConfigChanged();
        break;
      default:
        FASSERT(false, "Unknown notification");
      }
    }
    else /* Match task */
    {
      MatchTask* mtTsk = dynamic_cast<MatchTask*>(tsk);
      FASSERT(mtTsk != NULL, "Bad task type");
      eStatus status = ProcessTask(mtTsk);
      TODO; /* Add log in switch */
      switch (status)
      {
      case PROCESS_FAILED:
        {
          // Fill raw answer buffer
          FASSERT(mtTsk->m_error != NULL, "Null pointer m_error");
          FASSERT(mtTsk->m_rawAnswer == NULL, "m_rawAnwer should be null");
          unsigned errorDescLen = (unsigned) ::strlen(mtTsk->m_error->Description());
          mtTsk->m_rawAnswer = new char[errorDescLen + 1];
          (void) ::memcpy(mtTsk->m_rawAnswer, mtTsk->m_error->Description(), errorDescLen + 1);
        }
      }
      tsk->ChangeStatus(status); /* Change task status */
    }
    tsk->Release(); /* Release task */
    CORE_TREAT_TASK(tsk); /* Send task to core */
  }
  return EXIT_SUCCESS;
}

void  BasicEntity::onConfigChanged(void)
{
  /* Nothing to do */
}
