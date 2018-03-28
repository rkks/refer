/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Exception.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/#include <exception>
#include <string.h>
#include "Exception.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Exception::Exception(const char* msg)
{
  unsigned msgLen = (unsigned) ::strlen(msg);
  m_what = new char[msgLen + 1];
  (void) ::memcpy(m_what, msg, msgLen + 1);
}

Exception::~Exception(void)
{
  if (m_what) delete [] m_what;
}

const char* Exception::Description() const
{
  return m_what;
}
