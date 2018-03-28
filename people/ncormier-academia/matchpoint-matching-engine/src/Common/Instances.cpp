/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Interfaces.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __INSTANCES_CPP__
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Instances.h"
#include "MString.h"
#include "Attributes.h"
#include "Interfaces.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * ObjectInstance
 */
ObjectInstance::ObjectInstance(void)
{
  // Initialization
  m_id = new MString();
  m_attributes = new ObjectInstanceAttributes();
}

ObjectInstance::~ObjectInstance(void)
{
  // Cleanup
  if (m_id)         delete m_id;
  for (ObjectInstanceAttributes::const_iterator iter = m_attributes->begin(); 
       iter != m_attributes->end(); iter++)
    delete *iter;
  if (m_attributes) delete m_attributes;
}

const ObjectInstanceAttribute&  ObjectInstance::AttributeAtIndex(unsigned int index) const
{
  if (index >= (unsigned) m_attributes->size())
  {
    FASSERT(false, "Too high index");
    return InvalidObjectInstanceAttribute::Singleton();
  }
  ObjectInstanceAttributes::const_iterator iter;
  for (iter = m_attributes->begin(); index && iter != m_attributes->end(); iter++, index--);
  FASSERT(iter != m_attributes->end(), "Ooops");
  return **iter;
}

const ObjectInstanceAttribute&  ObjectInstance::AttributeWithID(const MString& id) const
{
  for (ObjectInstanceAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
  {
    if (id == (*iter)->Attribute().AttributeID())
      return **iter;
  }
  return InvalidObjectInstanceAttribute::Singleton();
}

unsigned  ObjectInstance::AttributeCount(void) const
{
  return (unsigned) m_attributes->size();
}

/*
 * MutableObjectInstance
 */
MutableObjectInstance::MutableObjectInstance(const ObjectInterface& obj)
{
  // Initialization
  *m_id = obj.ObjectID();
}

MutableObjectInstance::~MutableObjectInstance(void)
{
  /* CF ObjectInstance destructor */
}

void MutableObjectInstance::AddNewAttribute(const SQLAttribute* attr, const IAttributeValue& value)
{
  ObjectInstanceAttribute* newAttr = new ObjectInstanceAttribute(attr, value); /* Create a new attribute */
  m_attributes->push_back(newAttr); /* Add to attributes */
}

/*
 * MatchedObjectInstance
 */
MatchedObjectInstance::MatchedObjectInstance(void)
{
  // Initialization
  m_attributes = new MatchedObjectInstanceAttributes();
}

MatchedObjectInstance::~MatchedObjectInstance(void)
{
  // Cleanup
  for (MatchedObjectInstanceAttributes::const_iterator iter = m_attributes->begin(); 
       iter != m_attributes->end(); iter++)
    delete *iter;
  if (m_attributes) delete m_attributes;
}

const MatchedObjectInstanceAttribute&  MatchedObjectInstance::AttributeAtIndex(unsigned int index) const
{
  if (index >= (unsigned) m_attributes->size())
  {
    FASSERT(false, "Too high index");
    return InvalidMatchedObjectInstanceAttribute::Singleton();
  }
  MatchedObjectInstanceAttributes::const_iterator iter;
  for (iter = m_attributes->begin(); index && iter != m_attributes->end(); iter++, index--);
  FASSERT(iter != m_attributes->end(), "Ooops");
  return **iter;
}

const MatchedObjectInstanceAttribute&  MatchedObjectInstance::AttributeWithID(const MString& id) const
{
  for (MatchedObjectInstanceAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
  {
    if (id == (*iter)->Attribute().AttributeID())
      return **iter;
  }
  return InvalidMatchedObjectInstanceAttribute::Singleton();
}

unsigned  MatchedObjectInstance::AttributeCount(void) const
{
  return (unsigned) m_attributes->size();
}

/*
 * MutableMatchedObjectInstance
 */
MutableMatchedObjectInstance::MutableMatchedObjectInstance(const ObjectInstance& obj)
{
  for (unsigned i = 0, n = obj.AttributeCount(); i < n; i++)
    m_attributes->push_back(new MatchedObjectInstanceAttribute(obj.AttributeAtIndex(i)));
}

MutableMatchedObjectInstance::~MutableMatchedObjectInstance(void)
{
  /* CF ObjectInstance destructor */
}
