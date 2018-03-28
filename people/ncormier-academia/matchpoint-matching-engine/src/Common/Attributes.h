/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Attributes.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __ATTRIBUTES_H__
# define __ATTRIBUTES_H__

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
# define MAX_ENTITYID_SIZE                          256
# define MAX_COLUMN_SIZE                            256
# define IS_INVALID_OBJECT_ATTRIBUTE(o)             typeid(o) == typeid(InvalidObjectAttribute)
# define IS_INVALID_PROFILE_INTERFACE_ATTRIBUTE(o)  typeid(o) == typeid(InvalidProfileInterfaceAttribute)
# define IS_INVALID_OBJECT_INSTANCE_ATTRIBUTE(o)    typeid(o) == typeid(InvalidObjectInstanceAttribute)
# define IS_INVALID_SQL_ATTRIBUTE(o)                typeid(o) == typeid(InvalidSQLAttribute)
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <typeinfo>
#include "MList.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MapInterface;
class ISelector;
class MString;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*

 ------------------------------------------------------------------------------
 -                            AttributeValue
 ------------------------------------------------------------------------------

*/
/*
 * AttributeValueType
 */
class AttributeValueType
{
  /***************************************************************************/
  /* Public types definition                                                */
  /***************************************************************************/
public:
  enum eType
  {
    INTEGER = 0,
    FLOAT,
    STRING,
    LIST,
    UNDEFINED
  };
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  static const AttributeValueType*  Int(void);
  static const AttributeValueType*  Float(void);
  static const AttributeValueType*  String(void);
  static const AttributeValueType*  List(void);
  eType                             TypeOf(void) const;
protected:
                                    AttributeValueType(eType type);
  virtual                          ~AttributeValueType(void);
protected:
                                    AttributeValueType(void);
  void                              setType(int);
  void                              setType(float);
  void                              setType(MString);
  void                              setType(eType type);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  eType   m_type;
};
/*
 * IAttributeValue
 */
class IAttributeValue : public AttributeValueType
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                            IAttributeValue(void) {};
  virtual                  ~IAttributeValue(void) {};
  virtual IAttributeValue*  Clone(void) const = 0;
  virtual bool              IsEqualWith(const IAttributeValue& value) const = 0;
  virtual bool              IsBiggerThan(const IAttributeValue& value) const = 0;
};
/*
 * AttributeValue template
 */
template <typename T>
class AttributeValue : public IAttributeValue
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                    AttributeValue(void) { m_value = new T; setType(*m_value); } /* Init type */ 
  virtual          ~AttributeValue(void) { if (m_value) delete m_value; }
  const T&          Value(void) const { return *m_value; }
  IAttributeValue*  Clone(void) const { AttributeValue<T>* ret = new AttributeValue<T>(); ret->SetValue(*m_value); return ret; }
  void              SetValue(const T& value) const { *m_value = value; }
  bool              IsBiggerThan(const IAttributeValue& value) const
  { 
    FASSERT(TypeOf() == value.TypeOf(), "Compare two different types");
    return *m_value > *(static_cast<const AttributeValue<T>&>(value).m_value);
  }
  bool              IsEqualWith(const IAttributeValue& value) const
  {
    FASSERT(TypeOf() == value.TypeOf(), "Compare two different types");
    return *m_value == *(static_cast<const AttributeValue<T>&>(value).m_value);
  }
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  T*  m_value;
};
/*
 * ListAttributeValue
 */
class ListAttributeValue : public IAttributeValue
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  ListAttributeValue(void)
  {
    m_type = LIST;
    m_values = new MList<IAttributeValue*>();
  }
   
  virtual ~ListAttributeValue(void)
  {
    for (unsigned i = 0, n = m_values->Count(); i < n; i++)
      delete m_values->ElemAtIndex(i);
     if (m_values) delete m_values;
  }
  
  const IAttributeValue&  ValueAtIndex(unsigned index) const
  {
    return *(m_values->ElemAtIndex(index));
  }
  
  unsigned  Count(void) const
  {
    return m_values->Count();
  }
  
  IAttributeValue*  Clone(void) const
  {
    ListAttributeValue* ret = new ListAttributeValue();
    for (unsigned i = 0, n = Count(); i < n; i++)
      ret->PushValue(ValueAtIndex(i));
    return ret;
  }
  
  void  PushValue(const IAttributeValue& value) const 
  {
    IAttributeValue* newValue = value.Clone();
    m_values->PushBack(newValue);
  }
  
  bool  IsBiggerThan(const IAttributeValue& value) const
  {
    FASSERT(TypeOf() == value.TypeOf(), "Compare two different types");
    return Count() > static_cast<const ListAttributeValue&>(value).Count();
  }

  bool  IsEqualWith(const IAttributeValue& value) const
  {
    FASSERT(TypeOf() == value.TypeOf(), "Compare two different types");
    return Count() == static_cast<const ListAttributeValue&>(value).Count();
  }
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  MList<IAttributeValue*>*  m_values;
};


/*
 
 ------------------------------------------------------------------------------ 
 -                            SQLAttribute
 ------------------------------------------------------------------------------
 
 */
/*
 * SQLAttribute
 */
class SQLAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                              SQLAttribute(void);
public:
                              SQLAttribute(const MapInterface* parent, 
                                           const MString& id, 
                                           const MString& columnName,
                                           const AttributeValueType* type);
  virtual                    ~SQLAttribute(void);
  const MapInterface&         ParentInterface(void) const;
  const MString&              AttributeID(void) const;
  const MString&              ColumnName(void) const;
  const AttributeValueType&   Type(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  const MapInterface*         m_parent; /* Attributes is contained by */
  MString*                    m_id;
  MString*                    m_columnName;
  const AttributeValueType*   m_type;
};
/*
 * InvalidObjectAttribute
 */
class InvalidSQLAttribute : public SQLAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                  InvalidSQLAttribute(void);
  virtual                        ~InvalidSQLAttribute(void);
  static InvalidSQLAttribute&     Singleton(void);
};

/*
 
 ------------------------------------------------------------------------------ 
 -                            ObjectAttribute
 ------------------------------------------------------------------------------
 
 */
/*
 * ObjectAttribute
 */
class ObjectAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                              ObjectAttribute(void);
public:
                              ObjectAttribute(const SQLAttribute* attr, 
                                              const ISelector* selector,
                                              const AttributeValueType* type);
  virtual                    ~ObjectAttribute(void);
  const SQLAttribute&         Attribute(void) const;
  const ISelector&            Selector(void) const;
  const AttributeValueType&   Type(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*         m_attr;
  const ISelector*            m_selector;
  const AttributeValueType*   m_type;
};
/*
 * InvalidObjectAttribute
 */
class InvalidObjectAttribute : public ObjectAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                  InvalidObjectAttribute(void);
  virtual                        ~InvalidObjectAttribute(void);
  static InvalidObjectAttribute&  Singleton(void);
};
/*
 
 ------------------------------------------------------------------------------ 
 -                           ProfileAttribute
 ------------------------------------------------------------------------------
 
 */
/*
 * ProfileAttributeValueType
 */
class ProfileAttributeValueType
{
  /***************************************************************************/
  /* Public types definition                                                */
  /***************************************************************************/
public:
  enum eType
  {
    SINGLE = 0,
    RANGE,
    ENUM,
    UNDEFINED
  };
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  static const ProfileAttributeValueType*   Single(void);
  static const ProfileAttributeValueType*   Range(void);
  static const ProfileAttributeValueType*   Enum(void);
  eType                                     TypeOf(void) const;
protected:
                                            ProfileAttributeValueType(eType type);
                                            ProfileAttributeValueType(void);
  virtual                                  ~ProfileAttributeValueType(void);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  eType   m_type;
};
/*
 * IProfileAttributeValue
 */
class IProfileAttributeValue : public ProfileAttributeValueType
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                    IProfileAttributeValue(void) {};
  virtual                          ~IProfileAttributeValue(void) {};
  virtual IProfileAttributeValue*   Clone(void) const = 0;
};
/*
 * SingleProfileAttributeValue
 */
class SingleProfileAttributeValue : public IProfileAttributeValue
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                    SingleProfileAttributeValue(const IAttributeValue& value);
  virtual                          ~SingleProfileAttributeValue(void);
  virtual IProfileAttributeValue*   Clone(void) const;
  const IAttributeValue&            Value(void) const;
  bool                              IsEqualWith(const IAttributeValue& value) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  IAttributeValue* m_value;
};
/*
 * RangeProfileAttributeValue
 */
class RangeProfileAttributeValue : public IProfileAttributeValue
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                    RangeProfileAttributeValue(const IAttributeValue& lowValue,
                                                               const IAttributeValue& highValue);
  virtual                          ~RangeProfileAttributeValue(void);
  virtual IProfileAttributeValue*   Clone(void) const;
  const IAttributeValue&            LowValue(void) const;
  const IAttributeValue&            HighValue(void) const;
  bool                              IsInRange(const IAttributeValue& value) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  IAttributeValue* m_lowValue;
  IAttributeValue* m_highValue;
};
/*
 * EnumProfileAttributeValue
 */
class EnumProfileAttributeValue : public IProfileAttributeValue
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                    EnumProfileAttributeValue(const MList<const IAttributeValue*>& values);
  virtual                          ~EnumProfileAttributeValue(void);
  virtual IProfileAttributeValue*   Clone(void) const;
  const IAttributeValue&            ValueAtIndex(unsigned index) const;
  unsigned                          Count(void) const;
  bool                              IsInEnum(const IAttributeValue& value) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  MList<IAttributeValue*>*  m_values;
};
/*
 * ProfileInterfaceAttribute
 */
class ProfileInterfaceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                                  ProfileInterfaceAttribute(void);
public:
                                  ProfileInterfaceAttribute(const SQLAttribute* attr, 
                                                            const IProfileAttributeValue& value);
  virtual                        ~ProfileInterfaceAttribute(void);
  const SQLAttribute&             Attribute(void) const;
  const IProfileAttributeValue&   Value(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*             m_attr;
  const IProfileAttributeValue*   m_value;
};
/*
 * InvalidProfileInterfaceAttribute
 */
class InvalidProfileInterfaceAttribute : public ProfileInterfaceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                            InvalidProfileInterfaceAttribute(void);
  virtual                                  ~InvalidProfileInterfaceAttribute(void);
  static InvalidProfileInterfaceAttribute&  Singleton(void);
};

/*
 
 ------------------------------------------------------------------------------ 
 -                       ObjectInstanceAttribute
 ------------------------------------------------------------------------------
 
 */

/*
 * ObjectInstanceAttribute
 */
class ObjectInstanceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          ObjectInstanceAttribute(void);
                          ObjectInstanceAttribute(const ObjectInstanceAttribute& object);
public:
                          ObjectInstanceAttribute(const SQLAttribute* attr, 
                                                  const IAttributeValue& value);
  virtual                ~ObjectInstanceAttribute(void);
  const SQLAttribute&     Attribute(void) const;
  const IAttributeValue&  Value(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*     m_attr;
  const IAttributeValue*  m_value;
};
/*
 * InvalidObjectInstanceAttribute
 */
class InvalidObjectInstanceAttribute : public ObjectInstanceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                          InvalidObjectInstanceAttribute(void);
  virtual                                ~InvalidObjectInstanceAttribute(void);
  static InvalidObjectInstanceAttribute&  Singleton(void);
};

/*
 
 ------------------------------------------------------------------------------ 
 -                     MatchedObjectInstanceAttribute
 ------------------------------------------------------------------------------
 
 */
/*
 * MatchedObjectInstanceAttribute
 */
class MatchedObjectInstanceAttribute : public ObjectInstanceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          MatchedObjectInstanceAttribute(void);
public:
                          MatchedObjectInstanceAttribute(const ObjectInstanceAttribute& object);
  virtual                ~MatchedObjectInstanceAttribute(void);
                          /* TODO: MatchResult */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
                          /* TODO: MatchResult */
};
/*
 * InvalidMatchedObjectInstanceAttribute
 */
class InvalidMatchedObjectInstanceAttribute : public MatchedObjectInstanceAttribute
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                                  InvalidMatchedObjectInstanceAttribute(void);
  virtual                                        ~InvalidMatchedObjectInstanceAttribute(void);
  static InvalidMatchedObjectInstanceAttribute&   Singleton(void);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __ATTRIBUTES_H__ */
