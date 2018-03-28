/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UserRequest.cpp
 * Desc    :
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __USER_REQUEST_CPP__
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "UserRequest.h"
#include "MatchRules.h"
#include "Interfaces.h"
#include "MString.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * UserRequest
 */
UserRequest::UserRequest(void)
{
  // Initialization
  m_objects = new Objects();
  m_matchRules = NULL;
}

UserRequest::~UserRequest(void)
{
  // Cleanup
  for (Objects::const_iterator iter = m_objects->begin(); iter != m_objects->end(); iter++)
    delete *iter;
  if (m_objects)    delete m_objects;
  if (m_matchRules) delete m_matchRules;
}

unsigned  UserRequest::AnswerLimit(void) const
{
  return m_answerLimit;
}

unsigned  UserRequest::MinMatchPercentage(void) const
{
  return m_minMatchPercentage;
}

const ObjectInterface&  UserRequest::ObjectByID(const MString& id) const
{
  for (Objects::const_iterator iter = m_objects->begin(); iter != m_objects->end(); iter++)
  {
     if ((*iter)->ObjectID() == id)
      return **iter;
  }
  FASSERT(false, "Unknown id");
  return InvalidObjectInterface::Singleton();
}

const ObjectInterface&  UserRequest::ObjectAtIndex(unsigned index) const
{
  if (index >= (unsigned) m_objects->size())
  {
    FASSERT(false, "Too high index");
    return InvalidObjectInterface::Singleton();
  }
  Objects::const_iterator iter;
  for (iter = m_objects->begin(); index && iter != m_objects->end(); iter++)
     index--;
  FASSERT(iter != m_objects->end(), "Unknown id");
  return **iter;
}

unsigned  UserRequest::ObjectsCount(void) const
{
  return (unsigned) m_objects->size();
}

const MatchRules& UserRequest::Rules(void) const
{
  FASSERT(m_matchRules != NULL, "Uninitialized m_matchRules");
  return *m_matchRules;
}

/*
 * MutableUserRequest
 */
MutableUserRequest::MutableUserRequest(void)
{
}

MutableUserRequest::~MutableUserRequest(void)
{
}

void  MutableUserRequest::SetAnswerLimit(unsigned answerLimit)
{
  m_answerLimit = answerLimit;
}

void  MutableUserRequest::SetMinMatchPercentage(unsigned minMatchPercentage)
{
  m_minMatchPercentage = minMatchPercentage;
}

/*
 *  TO REMOVE
 */
void  MutableUserRequest::AddAllocatedObjectIntefaces(MutableObjectInterface* obj)
{
  m_objects->push_back(obj); /* Add object */
}

/*
 * TO REMOVE
 */
void  MutableUserRequest::SetAllocatedMatchRules(MutableMatchRules* rules)
{
  m_matchRules = rules;
}
