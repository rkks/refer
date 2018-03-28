/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UserErrror.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UserError.h"
#include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
UserError::UserError(const MString& error)
{
  // Members initialization
  m_str = new MString("Request failed: \n");
  *m_str += error;
}

UserError::~UserError(void)
{
  // Clean up
  if (m_str)  delete m_str;
}

const char* UserError::Description(void)
{
  return m_str->CharArray();
}
