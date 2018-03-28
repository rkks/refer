/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    DataEntity.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "DataEntity.h"
#include "MatchTask.h"
#include "SQLGenerator.h"
#include "Assert.h"
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
DataEntity::DataEntity(unsigned id) : BasicEntity(id)
{
}

DataEntity::~DataEntity(void)
{
}

const char* DataEntity::Description(void)
{
  return "DataEntity";
}

eStatus  DataEntity::ProcessTask(MatchTask* tsk)
{
  /*
  * Generate SQL request
  */
  try
  {
    SQLGenerator::FillAnswer(*tsk);
  }
  catch (const Exception& ex)
  {
    tsk->m_error = new UserError(ex.Description());
    return PROCESS_FAILED;
  }
  return PROCESS_SUCCEEDED;
}
