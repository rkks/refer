/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    NotificationTask.cpp
 * Desc    :    NotificationTask object implementation
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "NotificationTask.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
NotificationTask::NotificationTask(void)
{
  // Objects initialization
  m_notification = (eNotificationType) -1; /* Bad init */
}

NotificationTask::~NotificationTask(void)
{
}

void  NotificationTask::SetNotification(eNotificationType type)
{
  m_notification = type;
}

NotificationTask::eNotificationType NotificationTask::Notification(void)
{
  return m_notification;
}
