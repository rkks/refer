/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLAttributes.cpp
 * Desc    :    XML Attributes base implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "XMLAttributes.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
XMLAttributes::XMLAttributes()
{
}

XMLAttributes::~XMLAttributes()
{
  FreeAttributes();
}

bool XMLAttributes::GetValue(const MString &name, MString &o_val)
{
  AttributesMap::const_iterator iter = m_attrs.find(name);
  if (iter == m_attrs.end())
  {
    return (false);
  }
  o_val = iter->second;
  return (true);
}

void XMLAttributes::FreeAttributes()
{
  m_attrs.clear();
}
