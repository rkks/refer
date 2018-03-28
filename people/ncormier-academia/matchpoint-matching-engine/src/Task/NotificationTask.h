/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    NotificationTask.h
 * Desc    :    NotificationTask object declartion
 * Author  :    nico
 *****************************************************************************/

#ifndef __NOTIFICATION_TASK_H__
# define __NOTIFICATION_TASK_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "Task.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class NotificationTask : public Task
{
  friend class CoreEntity; /* CoreEntity handles creation*/
  /***************************************************************************/
  /* Public types definition                                                 */
  /***************************************************************************/
public:
  enum _eNotificationType
  {
    CONFIG_CHANGED = 0,
  };
  typedef enum _eNotificationType eNotificationType;  
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
private:
                      NotificationTask(void);
  virtual            ~NotificationTask(void);
public:
  void                SetNotification(eNotificationType type); /* NOT THREAD SAFE
                                                                * MUST BE CALLED BEFORE SEND */
  eNotificationType   Notification(void); /* Thread safe */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  eNotificationType   m_notification;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __NOTIFICATION_TASK_H__ */
