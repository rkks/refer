/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    IOEntity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "IOEntity.h"
#include "MatchTask.h"
#include "Exception.h"
#include "UserRequestAdapter.h"
#include "UserError.h"
#include "AnswerSerializer.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
IOEntity::IOEntity(unsigned id) : BasicEntity(id)
{
}

IOEntity::~IOEntity(void)
{
}

const char* IOEntity::Description(void)
{
  return "IOEntity";
}

eStatus  IOEntity::ProcessTask(MatchTask* tsk)
{
  // if tsk->MutableUserRequest == NULL
  //  Get raw request and parse it with xml parser
  //  1) Parse XML
  //  2) Check XML
  //  3) Fill UserRequest
  //      * Fill Objects through MapInterface
  //      * Fill MatchRules
  //      * Fill Others
  //  4) Thrown to core
  // else if tsk->MutableUserAnswer == NULL
  //  TODO
  // else
  //  FASSERT
  try
  {
    if (!tsk->m_mutableUserRequest) /* Generate user request */
      tsk->m_mutableUserRequest = UserRequestAdapter::Create(tsk->m_rawRequest, tsk->m_rawRequestLen);
    else /* Serialize answer */
    {
      /* TEMPORARY */
      MString output;
      AnswerSerializer serializer(output);
      serializer.Serialize(tsk->MatchEntityAnswer(), tsk->m_mutableUserRequest->Rules().AnswerOn());
      tsk->m_rawAnswer = new char[output.Size() + 1];
      tsk->m_rawAnswer[output.Size()] = 0;
      ::memcpy(tsk->m_rawAnswer, output.CharArray(), output.Size());
    }
  }
  catch (const Exception& ex)
  {
    tsk->m_error = new UserError(ex.Description());
    return PROCESS_FAILED;
  }
  return PROCESS_SUCCEEDED;
}
