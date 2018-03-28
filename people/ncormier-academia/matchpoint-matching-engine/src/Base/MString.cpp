/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MString.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __MSTRING_INTERNAL_TYPE std::string
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <string>
#include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MString::MString(void)
{
  m_str = new std::string();
}

MString::MString(const MString& str)
{
  m_str = new std::string(*str.m_str);
}

MString::MString(const char* str)
{
  m_str = new std::string(str);
}

MString::MString(const char* str, int size)
{
  m_str = new std::string(str, size);
}

MString::~MString(void)
{
  if (m_str)
    delete m_str;
}

char  MString::operator[](unsigned i) const
{
  return (*m_str)[i];
}

bool  MString::operator==(const MString& s) const
{
  return *s.m_str == *m_str;
}

bool  MString::operator!=(const MString& s) const
{
  return *s.m_str != *m_str;
}

bool  MString::operator>(const MString& s) const
{
  return *s.m_str > *m_str;
}

bool  MString::operator<(const MString& s) const
{
  return *s.m_str < *m_str;
}

MString& MString::operator+=(const MString& s)
{
  *m_str += *s.m_str;
  return *this;
}

MString& MString::operator=(const MString& s)
{
  *m_str = *s.m_str;
  return *this;
}

MString  MString::SubStr(unsigned pos /* = 0*/) const
{
  return MString(m_str->substr(pos).c_str());
}

MString  MString::SubStr(unsigned pos , unsigned npos) const
{
  return MString(m_str->substr(pos, npos).c_str());
}

const char* MString::CharArray(void) const
{
  return m_str->c_str();
}

int MString::ToInt(void) const
{
  return ::atoi(m_str->c_str());
}

unsigned int MString::Size(void) const
{
  return (unsigned)m_str->size();
}
