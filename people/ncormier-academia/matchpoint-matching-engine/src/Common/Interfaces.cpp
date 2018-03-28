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
#define __INTERFACES_CPP__
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Interfaces.h"
#include "Attributes.h"
#include "Selectors.h"
#include "Instances.h"
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
 * MapInterface
 */
MapInterface::MapInterface(void)
{
  // Initialization
  m_id = new MString();
  m_tableName = new MString();
  m_attributes = new LocalAttributes();
  m_subInterfaces = new SubMapInterface();
  m_localSelector = new NoneSelector();
}

MapInterface::~MapInterface(void)
{
  // Cleanup
  if (m_id)             delete m_id;
  if (m_tableName)      delete m_tableName;
  for (LocalAttributes::iterator  iter = m_attributes->begin(); iter != m_attributes->end();
        iter++)         delete *iter;
  if (m_attributes)     delete m_attributes;
  for (SubMapInterface::iterator  iter = m_subInterfaces->begin(); iter != m_subInterfaces->end();
        iter++)         delete *iter;
  if (m_subInterfaces)  delete m_subInterfaces;
  if (m_localSelector)  delete m_localSelector;
}

const MString&  MapInterface::MapID(void) const
{
  return *m_id;
}

const MString&  MapInterface::TableName(void) const
{
  return *m_tableName;
}

const SQLAttribute&   MapInterface::LocalAttributeByID(const MString& id) const
{
  SQLAttribute* ret = localAttributeByID(id);
  if (!ret)
    return InvalidSQLAttribute::Singleton();
  return *ret;
}

bool  MapInterface::AddAttributeToObject(const MString& entityid, const MString& id, const AttributeValueType* type, /* Inputs */
                                          MutableObjectInterface& output /* Output */ ) const
{
  LocalAttributes* attributes = NULL;
  ISelector*  selector = NULL;
  
  // Seek for entity
  if (entityid == *m_id) /* Local attribute */
  {
    attributes = m_attributes;
    selector = m_localSelector;
  }
  else /* Mayby in a sub entity ? */
  {
    for (SubMapInterface::const_iterator iter = m_subInterfaces->begin();
          iter != m_subInterfaces->end(); iter++)
    {
      if (entityid == *(*iter)->interface->m_id)
      {
        attributes = (*iter)->interface->m_attributes;
        selector = (*iter)->selector;
        break;
      }
    }
  }
  if (!attributes || !selector)
    return false; /* Entity not found */
  // Seek for attribute
  for (LocalAttributes::const_iterator iter = attributes->begin();
        iter != attributes->end(); iter++) /* Foreach attributes */
  {
    if (id == (*iter)->AttributeID())
    {
      output.AddNewAttribute(*iter, selector, type); /* Found ! */
      return true;
    }
  }
  return false; /* Not found */
}

SQLAttribute* MapInterface::localAttributeByID(const MString& id) const
{
  for (LocalAttributes::const_iterator iter = m_attributes->begin();
        iter != m_attributes->end(); iter++) /* Foreach attributes */
  {
    if (id == (*iter)->AttributeID())
      return *iter;
  }
  return NULL;
}

/*
 * MutableMapInterface
 */
MutableMapInterface::MutableMapInterface(const MString& id, const MString& tableName)
{
  // Initialization
  *m_id = id;
  *m_tableName = tableName;
}

MutableMapInterface::~MutableMapInterface(void)
{
  /* CF MapInterface destructor */
}

/*
 * Verifier les doublons ?!! dans les Add*
 */
void  MutableMapInterface::AddLocalAttribute(const MString& id, const MString& columnName, const AttributeValueType* type)
{
  SQLAttribute* newAttr = new SQLAttribute(this, id, columnName, type); /* Create a new attribute */
  m_attributes->push_back(newAttr); /* Add it */
}

void  MutableMapInterface::AddSubMapInterfaceWithBelongsToLink(const MapInterface* target, 
                                                               const MString& localAttributeID,
                                                               const MString& targetAttributeID)
{
  // Create sub interface
  subInterface* newSubInterface = new subInterface();
  newSubInterface->interface = target;
  SQLAttribute* localAttribute = localAttributeByID(localAttributeID); /* Get local attribute */
  SQLAttribute* targetAttribute = target->localAttributeByID(targetAttributeID); /* Get target attribute */
  newSubInterface->selector = new BelongsToSelector(localAttribute, targetAttribute); /* Create selector */
  m_subInterfaces->push_back(newSubInterface); /* Add to sub interfaces */
}

void  MutableMapInterface::AddSubMapInterfaceWithHABTMLink(const MapInterface* target,
                                                           const MapInterface* inter, 
                                                           const MString& localAttributeID,
                                                           const MString& localToInterAttributeID,
                                                           const MString& interToTargetAttributeID,
                                                           const MString& targetAttributeID
                                                           )
{
  // Create sub interface
  subInterface* newSubInterface = new subInterface();
  newSubInterface->interface = target;
  SQLAttribute* localAttribute = localAttributeByID(localAttributeID); /* Get local attribute */
  SQLAttribute* localToInterAttribute = inter->localAttributeByID(localToInterAttributeID); /* Get local to inter attribute */
  SQLAttribute* targetAttribute = target->localAttributeByID(targetAttributeID); /* Get target attribute */
  SQLAttribute* interToTargetAttribute = inter->localAttributeByID(interToTargetAttributeID); /* Get inter to target attribute */
  newSubInterface->selector = new HABTMSelector(localAttribute, localToInterAttribute,
                                                interToTargetAttribute, targetAttribute); /* Create selector */
  m_subInterfaces->push_back(newSubInterface); /* Add to sub interfaces */
}

/*
 * InvalidObjectInterface
 */
InvalidMapInterface::InvalidMapInterface(void)
{
}

InvalidMapInterface::~InvalidMapInterface(void)
{
}

InvalidMapInterface&  InvalidMapInterface::Singleton(void)
{
  static InvalidMapInterface* singleton = NULL;
  if (!singleton)
    singleton = new InvalidMapInterface();
  return *singleton;
}

/*
 * ObjectInterface
 */
ObjectInterface::ObjectInterface(void)
{
  // Initialization
  m_id = new MString();
  m_attributes = new ObjectAttributes();
}

ObjectInterface::~ObjectInterface(void)
{
  // Cleanup
  if (m_id)         delete m_id;
  for (ObjectAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
    delete *iter;
  if (m_attributes) delete m_attributes;
}

const MString&  ObjectInterface::ObjectID(void) const
{
  return *m_id;
}

const ObjectAttribute&  ObjectInterface::AttributeAtIndex(unsigned int index) const
{
  if (index >= (unsigned) m_attributes->size())
  {
    FASSERT(false, "Too high index");
    return InvalidObjectAttribute::Singleton();
  }
  ObjectAttributes::const_iterator iter;
  for (iter = m_attributes->begin(); index && iter != m_attributes->end(); iter++, index--);
  FASSERT(iter != m_attributes->end(), "Ooops");
  return **iter;
}

const ObjectAttribute&  ObjectInterface::AttributeWithID(const MString& id) const
{
  for (ObjectAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
  {
    if (id == (*iter)->Attribute().AttributeID())
      return **iter;
  }
  return InvalidObjectAttribute::Singleton();
}

unsigned  ObjectInterface::AttributeCount(void) const
{
  return (unsigned) m_attributes->size();
}

bool  ObjectInterface::AddAttributeToInstance(const MString& id, const IAttributeValue& value, /* Inputs */
                                              MutableObjectInstance& output /* Output */ ) const
{
  const ObjectAttribute& attr = AttributeWithID(id);
  if (IS_INVALID_OBJECT_ATTRIBUTE(attr))
    return false;
  output.AddNewAttribute(&attr.Attribute(), value);
  return true;
}

/*
 * MutableObjectInterface
 */
MutableObjectInterface::MutableObjectInterface(const MString& id)
{
  // Initialization
  *m_id = id;
}

MutableObjectInterface::~MutableObjectInterface(void)
{
  /* CF ObjectInterface destructor */
}

void MutableObjectInterface::AddNewAttribute(const SQLAttribute* attr, 
                                             const ISelector* selector, 
                                             const AttributeValueType* type)
{
  ObjectAttribute* newAttr = new ObjectAttribute(attr, selector, type); /* Create a new attribute */
  m_attributes->push_back(newAttr); /* Add to attributes */
}

/*
 * InvalidObjectInterface
 */
InvalidObjectInterface::InvalidObjectInterface(void)
{
}

InvalidObjectInterface::~InvalidObjectInterface(void)
{
}

InvalidObjectInterface&  InvalidObjectInterface::Singleton(void)
{
  static InvalidObjectInterface* singleton = NULL;
  if (!singleton)
    singleton = new InvalidObjectInterface();
  return *singleton;
}

/*
 * ProfileInterface
 */
ProfileInterface::ProfileInterface(void)
{
  // Initialization
  m_attributes = new ProfileAttributes();
}

ProfileInterface::~ProfileInterface(void)
{
  // Cleanup
  for (ProfileAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
    delete *iter;
  if (m_attributes) delete m_attributes;
}

const ProfileInterfaceAttribute&  ProfileInterface::AttributeAtIndex(unsigned int index) const
{
  if (index >= (unsigned) m_attributes->size())
  {
    FASSERT(false, "Too high index");
    return InvalidProfileInterfaceAttribute::Singleton();
  }
  ProfileAttributes::const_iterator iter;
  for (iter = m_attributes->begin(); index && iter != m_attributes->end(); iter++, index--);
  FASSERT(iter != m_attributes->end(), "Ooops");
  return **iter;
}

const ProfileInterfaceAttribute&  ProfileInterface::AttributeWithID(const MString& id) const
{
  for (ProfileAttributes::const_iterator iter = m_attributes->begin(); 
        iter != m_attributes->end(); iter++)
  {
    if (id == (*iter)->Attribute().AttributeID())
      return **iter;
  }
  return InvalidProfileInterfaceAttribute::Singleton();
}

unsigned  ProfileInterface::AttributeCount(void) const
{
  return (unsigned) m_attributes->size();
}

/*
 * MutableObjectInterface
 */
MutableProfileInterface::MutableProfileInterface(void)
{
}

MutableProfileInterface::~MutableProfileInterface(void)
{
}
 
void MutableProfileInterface::AddAttribute(const SQLAttribute* attr, const IProfileAttributeValue& value)
{
  ProfileInterfaceAttribute* newAttr = new ProfileInterfaceAttribute(attr, value);
  m_attributes->push_back(newAttr);
}
