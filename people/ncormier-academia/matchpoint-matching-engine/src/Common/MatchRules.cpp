/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchRules.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MatchRules.h"
#include "MString.h"
#include "MList.h"
#include "Assert.h"
#include "Interfaces.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * MatchRules
 */
MatchRules::MatchRules(void)
{
  // Objects initialization
  m_name = NULL;
  m_requestOn = NULL;
  m_answerOn = NULL;
  m_profileInterfaces = new MList<MutableProfileInterface*>();
  
}

MatchRules::~MatchRules(void)
{
  // Objects cleanup
  if (m_name)             delete m_name;
  for (unsigned i = 0, n = m_profileInterfaces->Count(); i < n; i++)
    delete m_profileInterfaces->ElemAtIndex(i);
  if (m_profileInterfaces) delete m_profileInterfaces;
}

const MString&  MatchRules::Name(void) const
{
  return *m_name;
}

const ObjectInterface&  MatchRules::RequestOn(void) const
{
  return *m_requestOn;
}

const ObjectInterface&  MatchRules::AnswerOn(void) const
{
  return *m_answerOn;
}

/*
 * TO REMOVE
 */
const ProfileInterface& MatchRules::ProfileAtIndex(unsigned index) const
{
  return *(m_profileInterfaces->ElemAtIndex(index));
}

unsigned  MatchRules::ProfileCount(void) const
{
  return m_profileInterfaces->Count();
}

/*
 * MutableMatchRules
 * TO CHANGE !!
 */
MutableMatchRules::MutableMatchRules(void)
{
}

MutableMatchRules::~MutableMatchRules(void)
{
}

void  MutableMatchRules::SetName(const MString& name)
{
  m_name = new MString(name);
}

void  MutableMatchRules::SetRequestOn(const ObjectInterface& requestOn)
{
  m_requestOn = const_cast<ObjectInterface*>(&requestOn);
}

void  MutableMatchRules::SetAnswerOn(const ObjectInterface& answerOn)
{
  m_answerOn = const_cast<ObjectInterface*>(&answerOn);
}

void  MutableMatchRules::AddAllocatedProfile(MutableProfileInterface* profile)
{
  m_profileInterfaces->PushBack(profile);
}
