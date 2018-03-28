/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchEntity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MatchEntity.h"
#include "Matcher.h"
#include "MatchTask.h"
#include "Exception.h"
#include "UserError.h"
#include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MatchEntity::MatchEntity(unsigned id) : BasicEntity(id)
{
}

MatchEntity::~MatchEntity(void)
{
}

const char* MatchEntity::Description(void)
{
  return "MatchEntity";
}

eStatus  MatchEntity::ProcessTask(MatchTask* tsk)
{
  try
  {
    Matcher::Process(*tsk);
  }
  catch (const Exception& ex)
  {
    tsk->m_error = new UserError(ex.Description());
    return PROCESS_FAILED;
  }
  return PROCESS_SUCCEEDED;
}
