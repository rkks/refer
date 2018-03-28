/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    XMLErrorHandler.cpp
* Desc    :    XML Error Handler base implementation
* Author  :    mat
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifndef _WIN32
#  include "LibXMLErrorHandler.h"
# else
#  include "MSXMLErrorHandler.h"
# endif

#include "XMLErrorHandler.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
XMLErrorHandler::XMLErrorHandler()
{
  m_errorFlag = false;
  m_error = NULL;
  m_line = -1;
  m_column = -1;
}

XMLErrorHandler::~XMLErrorHandler()
{
  if (m_error != NULL)
  {
    delete m_error;
  }
}

void XMLErrorHandler::SetErrorMessage(MString error)
{
  if (m_error != NULL)
  {
    delete m_error;
  }
  m_errorFlag = true;
  m_error = new MString(error);
}

bool XMLErrorHandler::IsErrorSet(void)
{
  return (m_errorFlag);
}

const char *XMLErrorHandler::GetErrorMessage()
{
  if (m_error == NULL)
    return ("");
  return (m_error->CharArray());
}

void XMLErrorHandler::SetErrorLine(int line)
{
  m_line = line;
}

int XMLErrorHandler::GetErrorLine()
{
  return (m_line);
}

void XMLErrorHandler::SetErrorColumn(int column)
{
  m_column = column;
}

int XMLErrorHandler::GetErrorColumn()
{
  return (m_column);
}
