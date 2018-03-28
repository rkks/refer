/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLAttributes.cpp
 * Desc    :    XML Attribute Unix implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <string.h>
#include "LibXMLAttributes.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

LibXMLAttributes::LibXMLAttributes(const char **attrs)
{
   AddAttributes(attrs);
}

void LibXMLAttributes::AddAttributes(const char **attrs)
{
  for (int i = 0; attrs[i] != NULL; i += 2)
    AddAttribute(attrs[i], attrs[i + 1]);
}

void LibXMLAttributes::AddAttribute(const char *name, const char *value)
{
  m_attrs[strdup(name)] = strdup(value);
}

LibXMLAttributes::~LibXMLAttributes()
{
}
