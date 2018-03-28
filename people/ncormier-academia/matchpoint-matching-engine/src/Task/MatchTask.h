/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchTask.h
 * Desc    :    MatchTask object declartion
 *              Temporary: keep properties access public ?
 * Author  :    nico
 *****************************************************************************/

#ifndef __MATCH_TASK_H__
# define __MATCH_TASK_H__

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
/* Forward definitions                                                       */
/*****************************************************************************/
class Socket;
class Error;
class UserRequest;
class MutableUserRequest;
class DataAnswer;
class MutableDataAnswer;
class MatchAnswer;
class MutableMatchAnswer;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class MatchTask : public Task
{
  friend class CoreEntity; /* CoreEntity handles creation*/
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
private:
                        MatchTask(void);
  virtual              ~MatchTask(void);
public:
  const UserRequest&    Request(void) const;
  const DataAnswer&     DataEntityAnswer(void) const;
  const MatchAnswer&    MatchEntityAnswer(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
public:
  Socket*               m_clientSocket;         /* Used by network entity to tranfert data */
  char*                 m_rawRequest;           /* Contain user's request as received */
  unsigned              m_rawRequestLen;
  char*                 m_rawAnswer;            /* Contain engine's answer as send */
  Error*                m_error;                /* If m_status is equal to failed, m_error is not null
                                                   and contains failure details */
  MutableUserRequest*   m_mutableUserRequest;   /* Only for IOEntity usage */
  MutableDataAnswer*    m_mutableDataAnswer;    /* Only for DataEntity usage */
  MutableMatchAnswer*   m_mutableMatchAnswer;   /* Only for MatchEntity usage */
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __MATCH_TASK_H__ */
