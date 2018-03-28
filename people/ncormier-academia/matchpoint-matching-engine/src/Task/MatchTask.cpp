/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchTask.cpp
 * Desc    :    MatchTask object implementation
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/#include "MatchTask.h"
#include "Socket.h"
#include "Error.h"
#include "Types.h"
#include "UserRequest.h"
#include "DataAnswer.h"
#include "MatchAnswer.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MatchTask::MatchTask(void)
{
  // Objects initialization
  m_clientSocket = NULL;
  m_rawRequest = NULL;
  m_rawRequestLen = 0;
  m_rawAnswer = NULL;
  m_error = NULL;
  m_mutableUserRequest = NULL;
  m_mutableDataAnswer = NULL;
  m_mutableMatchAnswer = NULL;
}

MatchTask::~MatchTask(void)
{
  // Objects destruction
  if (m_clientSocket)         delete m_clientSocket;
  if (m_rawRequest)           delete [] m_rawRequest;
  if (m_rawAnswer)            delete [] m_rawAnswer;
  if (m_error)                delete m_error;
  if (m_mutableUserRequest)   delete m_mutableUserRequest;
  if (m_mutableDataAnswer)    delete m_mutableDataAnswer;
  if (m_mutableMatchAnswer)   delete m_mutableMatchAnswer;
}

const UserRequest&  MatchTask::Request(void) const
{
  FASSERT(m_mutableUserRequest != NULL, "UserRequest object is null");
  return *m_mutableUserRequest;
}

const DataAnswer& MatchTask::DataEntityAnswer(void) const
{
  FASSERT(m_mutableDataAnswer != NULL, "DataAnswer object is null");
  return *m_mutableDataAnswer;
}

const MatchAnswer&  MatchTask::MatchEntityAnswer(void) const
{
  FASSERT(m_mutableMatchAnswer != NULL, "MatchAnswer object is null");
  return *m_mutableMatchAnswer;
}
