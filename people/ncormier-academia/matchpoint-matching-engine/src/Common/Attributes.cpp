/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Config.cpp
 * Desc    :    Warning: SQLAttribute, MapAttribute, ObjectAttribute,
 *              and ProfileAttribute are just containers !
 *              There is no allocation in it;
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __ATTRIBUTES_CPP__
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <string.h>
#include "Attributes.h"
#include "MString.h"
#include "Interfaces.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * AttributeValueType
 */
 
const AttributeValueType* AttributeValueType::Int(void)
{
  static AttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new AttributeValueType(INTEGER);
  return singleton;
}

const AttributeValueType* AttributeValueType::Float(void)
{
  static AttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new AttributeValueType(FLOAT);
  return singleton;
}

const AttributeValueType* AttributeValueType::String(void)
{
  static AttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new AttributeValueType(STRING);
  return singleton;
}

const AttributeValueType* AttributeValueType::List(void)
{
  static AttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new AttributeValueType(LIST);
  return singleton;
}

AttributeValueType::AttributeValueType(eType type)
{
  m_type = type;
}

AttributeValueType::AttributeValueType(void)
{
  m_type = UNDEFINED;
}

AttributeValueType::~AttributeValueType(void)
{

}

AttributeValueType::eType AttributeValueType::TypeOf(void) const
{
  return m_type;
}

void  AttributeValueType::setType(int)
{ 
  m_type = INTEGER; 
}

void  AttributeValueType::setType(float)
{
  m_type = FLOAT;
}

void  AttributeValueType::setType(MString)
{
  m_type = STRING;
}

/*
 * SQLAttribute
 */
SQLAttribute::SQLAttribute(void)
{
  m_parent = NULL;
  m_id = NULL;
  m_columnName = NULL;
  m_type = NULL;  
}

SQLAttribute::SQLAttribute(const MapInterface* parent, const MString& id, const MString& columnName, const AttributeValueType* type)
{
  m_parent = parent;
  m_id = new MString(id);
  m_columnName = new MString(columnName);
  m_type = type;  
}

SQLAttribute::~SQLAttribute(void)
{
  // Cleanup
  if (m_id)         delete m_id;
  if (m_columnName) delete m_columnName;
}

const MapInterface& SQLAttribute::ParentInterface(void) const
{
  return *m_parent;
}

const MString& SQLAttribute::AttributeID(void) const
{
  return *m_id;
}

const MString& SQLAttribute::ColumnName(void) const
{
  return *m_columnName;
}

const AttributeValueType& SQLAttribute::Type(void) const
{
  return *m_type;
}

/*
 * InvalidSQLAttribute
 */
InvalidSQLAttribute::InvalidSQLAttribute(void)
{
}

InvalidSQLAttribute::~InvalidSQLAttribute(void)
{
}

InvalidSQLAttribute&  InvalidSQLAttribute::Singleton(void)
{
  static InvalidSQLAttribute* singleton = NULL;
  if (!singleton)
    singleton = new InvalidSQLAttribute();
  return *singleton;
}

/*
 * ObjectAttribute
 */
ObjectAttribute::ObjectAttribute(void)
{
  m_attr = NULL;
  m_selector = NULL;
  m_type = NULL;
}

ObjectAttribute::ObjectAttribute(const SQLAttribute* attr, const ISelector* selector, const AttributeValueType* type)
{
  m_attr = attr;
  m_selector = selector;
  m_type = type;
}

ObjectAttribute::~ObjectAttribute(void)
{

}

const SQLAttribute& ObjectAttribute::Attribute(void) const
{
  return *m_attr;
}

const ISelector&  ObjectAttribute::Selector(void) const
{
  return *m_selector;
}

const AttributeValueType& ObjectAttribute::Type(void) const
{
  return *m_type;
}

/*
 * InvalidObjectAttribute
 */
InvalidObjectAttribute::InvalidObjectAttribute(void)
{
}

InvalidObjectAttribute::~InvalidObjectAttribute(void)
{
}

InvalidObjectAttribute&  InvalidObjectAttribute::Singleton(void)
{
  static InvalidObjectAttribute* singleton = NULL;
  if (!singleton)
    singleton = new InvalidObjectAttribute();
  return *singleton;
}

/*
 * ProfileAttributeValueType
 */
const ProfileAttributeValueType*  ProfileAttributeValueType::Single(void)
{
  static ProfileAttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new ProfileAttributeValueType(SINGLE);
  return singleton;
}

const ProfileAttributeValueType*  ProfileAttributeValueType::Range(void)
{
  static ProfileAttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new ProfileAttributeValueType(RANGE);
  return singleton;
}

const ProfileAttributeValueType*  ProfileAttributeValueType::Enum(void)
{
  static ProfileAttributeValueType* singleton = NULL;
  if (!singleton)
    singleton = new ProfileAttributeValueType(ENUM);
  return singleton;
}

ProfileAttributeValueType::eType ProfileAttributeValueType::TypeOf(void) const
{
  return m_type;
}

ProfileAttributeValueType::ProfileAttributeValueType(eType type)
{
  m_type = type;
}

ProfileAttributeValueType::ProfileAttributeValueType(void)
{
  m_type = UNDEFINED;
}

ProfileAttributeValueType::~ProfileAttributeValueType(void)
{
}

/*
 * SingleProfileAttributeValue
 */
SingleProfileAttributeValue::SingleProfileAttributeValue(const IAttributeValue& value)
{
  m_type = SINGLE; 
  m_value = value.Clone();
}

SingleProfileAttributeValue::~SingleProfileAttributeValue(void)
{
  if (m_value)  delete m_value;
}

IProfileAttributeValue* SingleProfileAttributeValue::Clone(void) const
{
  return new SingleProfileAttributeValue(*m_value);
}

const IAttributeValue&  SingleProfileAttributeValue::Value(void) const
{
  return *m_value;
}

bool  SingleProfileAttributeValue::IsEqualWith(const IAttributeValue& value) const
{
  return value.IsEqualWith(*m_value);
}
/*
 * RangeProfileAttributeValue
 */
RangeProfileAttributeValue::RangeProfileAttributeValue(const IAttributeValue& lowValue, const IAttributeValue& highValue)
{
  m_type = RANGE;
  m_lowValue = lowValue.Clone();
  m_highValue = highValue.Clone();
}

RangeProfileAttributeValue::~RangeProfileAttributeValue(void)
{
  if (m_lowValue)   delete m_lowValue;
  if (m_highValue)  delete m_highValue;
}

IProfileAttributeValue* RangeProfileAttributeValue::Clone(void) const
{
  return new RangeProfileAttributeValue(*m_lowValue, *m_highValue);
}

const IAttributeValue&  RangeProfileAttributeValue::LowValue(void) const
{
  return *m_lowValue;
}

const IAttributeValue& RangeProfileAttributeValue::HighValue(void) const
{
  return *m_highValue;
}

bool RangeProfileAttributeValue::IsInRange(const IAttributeValue& value) const
{
  return value.IsBiggerThan(*m_lowValue) && !value.IsBiggerThan(*m_highValue);
}

/*
 * EnumProfileAttributeValue
 */
EnumProfileAttributeValue::EnumProfileAttributeValue(const MList<const IAttributeValue*>& values)
{
  m_type = ENUM;
  m_values = new MList<IAttributeValue*>();
  for (unsigned i = 0, n = values.Count(); i < n; i++)
    m_values->PushBack(values.ElemAtIndex(i)->Clone());
}

EnumProfileAttributeValue::~EnumProfileAttributeValue(void)
{
  for (unsigned i = 0, n = m_values->Count(); i < n; i++)
    delete m_values->ElemAtIndex(i);
  if (m_values) delete m_values;
}

IProfileAttributeValue* EnumProfileAttributeValue::Clone(void) const
{
  MList<const IAttributeValue*> cloner;
  for (unsigned i = 0, n = m_values->Count(); i < n; i++)
    cloner.PushBack(m_values->ElemAtIndex(i));
  return new EnumProfileAttributeValue(cloner);
}

const IAttributeValue&  EnumProfileAttributeValue::ValueAtIndex(unsigned index) const
{
  return *m_values->ConstElemAtIndex(index);
}

unsigned  EnumProfileAttributeValue::Count(void) const
{
  return m_values->Count();
}

bool EnumProfileAttributeValue::IsInEnum(const IAttributeValue& value) const
{
  for (unsigned i = 0, n = m_values->Count(); i < n; i++)
  {
    if (value.IsEqualWith(*(m_values->ElemAtIndex(i))))
        return true;
  }
  return false;
}

/*
 * ProfileInterfaceAttribute
 */
ProfileInterfaceAttribute::ProfileInterfaceAttribute(void)
{
  m_attr = NULL;
  m_value = NULL;
}

ProfileInterfaceAttribute::ProfileInterfaceAttribute(const SQLAttribute* attr, const IProfileAttributeValue& value)
{
  m_attr = attr;
  m_value = value.Clone();
}

ProfileInterfaceAttribute::~ProfileInterfaceAttribute(void)
{
  if (m_value)  delete m_value;
  /* m_attr is not mine */
}

const SQLAttribute& ProfileInterfaceAttribute::Attribute(void) const
{
  return *m_attr;
}

const IProfileAttributeValue& ProfileInterfaceAttribute::Value(void) const
{
  return *m_value;
}

/*
 * InvalidProfileInterfaceAttribute
 */
InvalidProfileInterfaceAttribute::InvalidProfileInterfaceAttribute(void)
{
}

InvalidProfileInterfaceAttribute::~InvalidProfileInterfaceAttribute(void)
{
}

InvalidProfileInterfaceAttribute& InvalidProfileInterfaceAttribute::Singleton(void)
{
  static InvalidProfileInterfaceAttribute* singleton = NULL;
  if (!singleton)
    singleton = new InvalidProfileInterfaceAttribute();
  return *singleton;
}

/*
 * ObjectInstanceAttribute
 */
ObjectInstanceAttribute::ObjectInstanceAttribute(void)
{
  m_attr = NULL;
  m_value = NULL;
}

ObjectInstanceAttribute::ObjectInstanceAttribute(const ObjectInstanceAttribute& object)
{
  m_attr = object.m_attr;
  m_value = object.m_value->Clone();
}

ObjectInstanceAttribute::ObjectInstanceAttribute(const SQLAttribute* attr, const IAttributeValue& value)
{
  m_attr = attr;
  m_value = value.Clone();
}

ObjectInstanceAttribute::~ObjectInstanceAttribute(void)
{
  if (m_value)  delete m_value;
}

const SQLAttribute& ObjectInstanceAttribute::Attribute(void) const
{
  return *m_attr;
}

const IAttributeValue& ObjectInstanceAttribute::Value(void) const
{
  return *m_value;
}

/*
 * InvalidObjectInstanceAttribute
 */
InvalidObjectInstanceAttribute::InvalidObjectInstanceAttribute(void)
{
}

InvalidObjectInstanceAttribute::~InvalidObjectInstanceAttribute(void)
{
}

InvalidObjectInstanceAttribute&  InvalidObjectInstanceAttribute::Singleton(void)
{
  static InvalidObjectInstanceAttribute* singleton = NULL;
  if (!singleton)
    singleton = new InvalidObjectInstanceAttribute();
  return *singleton;
}


/*
 * MatchedObjectInstanceAttribute
 */
MatchedObjectInstanceAttribute::MatchedObjectInstanceAttribute(void)
{
}

MatchedObjectInstanceAttribute::MatchedObjectInstanceAttribute(const ObjectInstanceAttribute& object) : ObjectInstanceAttribute(object)
{
  /* TODO */
}

MatchedObjectInstanceAttribute::~MatchedObjectInstanceAttribute(void)
{
  /* TODO */
}

/*
 * InvalidMatchedObjectInstanceAttribute
 */
InvalidMatchedObjectInstanceAttribute::InvalidMatchedObjectInstanceAttribute(void)
{
}

InvalidMatchedObjectInstanceAttribute::~InvalidMatchedObjectInstanceAttribute(void)
{
}

InvalidMatchedObjectInstanceAttribute&  InvalidMatchedObjectInstanceAttribute::Singleton(void)
{
  static InvalidMatchedObjectInstanceAttribute* singleton = NULL;
  if (!singleton)
    singleton = new InvalidMatchedObjectInstanceAttribute();
  return *singleton;
}
