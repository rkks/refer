/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MapInterface.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __INTERFACES_H__
# define __INTERFACES_H__

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
# define IS_INVALID_MAP_INTERFACE(o)    typeid(o) == typeid(InvalidMapInterface)
# define IS_INVALID_OBJECT_INTERFACE(o) typeid(o) == typeid(InvalidObjectInterface)
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifdef __INTERFACES_CPP__
#  include <list>
# endif /* __INTERFACES_CPP__ */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MutableObjectInterface;
class MutableObjectInstance;
class MapAttribute;
class ObjectAttribute;
class ProfileInterfaceAttribute;
class IAttributeValue;
class AttributeValueType;
class MString;
class SQLAttribute;
class ISelector;
class IProfileAttributeValue;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
 * MapInterface
 */
class MapInterface
{
  friend class MutableMapInterface;
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                        MapInterface(void);
  virtual              ~MapInterface(void);
  SQLAttribute*         localAttributeByID(const MString& id) const;
public:
  const MString&        MapID(void) const;
  const MString&        TableName(void) const;
  const SQLAttribute&   LocalAttributeByID(const MString& id) const;
  bool                  AddAttributeToObject(const MString& entityid, const MString& id, const AttributeValueType* type, /* Inputs */
                                             MutableObjectInterface& output /* Output */ ) const;
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
protected:
# ifdef __INTERFACES_CPP__
  typedef std::list<SQLAttribute*> LocalAttributes; /* Attributes are mine */
  struct subInterface
  {
    ISelector*            selector;
    const MapInterface*   interface;
  };
  typedef std::list<subInterface*> SubMapInterface; /* ISelector are mine */
# else
  typedef void LocalAttributes, SubMapInterface;  
# endif /* __INTERFACES_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MString*          m_id;
  MString*          m_tableName;
  LocalAttributes*  m_attributes; /* Local attributes */
  ISelector*        m_localSelector;
  SubMapInterface*  m_subInterfaces; /* Sub objects */
};
class MutableMapInterface : public MapInterface
{
  /* 
      TODO XML integration
      Contains by Config.cpp
      TO COMPLETE: Link types
  */
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
              MutableMapInterface(const MString& id, const MString& tableName);
  virtual    ~MutableMapInterface(void);
  void        AddLocalAttribute(const MString& id, const MString& columnName, 
                                const AttributeValueType* type);
  void        AddSubMapInterfaceWithBelongsToLink(const MapInterface* target, 
                                              const MString& localAttributeID,
                                              const MString& targetAttributeID);
  void        AddSubMapInterfaceWithHABTMLink(const MapInterface* target,
                                              const MapInterface* inter, 
                                              const MString& localAttributeID,
                                              const MString& localToInterAttributeID,
                                              const MString& interToTargetAttributeID,
                                              const MString& targetAttributeID);
};
/*
 * InvalidMapInterface
 */
class InvalidMapInterface : public MapInterface
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                          InvalidMapInterface(void);
  virtual                                ~InvalidMapInterface(void);
  static InvalidMapInterface&             Singleton(void);
};
/*
 * ObjectInterface
 */
class ObjectInterface
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          ObjectInterface(void);
  virtual                ~ObjectInterface(void);
public:
  const MString&          ObjectID(void) const;
  const ObjectAttribute&  AttributeAtIndex(unsigned int index) const;
  const ObjectAttribute&  AttributeWithID(const MString& id) const;
  unsigned                AttributeCount(void) const;
  bool                    AddAttributeToInstance(const MString& id, const IAttributeValue& value, /* Inputs */
                                                 MutableObjectInstance& output /* Output */ ) const;
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
protected:
# ifdef __INTERFACES_CPP__
  typedef std::list<ObjectAttribute*> ObjectAttributes; /* Attributes are mine */
# else
  typedef void ObjectAttributes;
# endif /* __INTERFACES_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MString*            m_id;
  ObjectAttributes*   m_attributes;
};
class MutableObjectInterface : public ObjectInterface
{
  /*  
      TODO XML integration
      Contains by Request.cpp
  */
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                MutableObjectInterface(const MString& id);
  virtual      ~MutableObjectInterface(void);
  void          AddNewAttribute(const SQLAttribute* attr, const ISelector* selector, 
                                const AttributeValueType* type);
};
/*
 * InvalidObjectInterface
 */
class InvalidObjectInterface : public ObjectInterface
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                          InvalidObjectInterface(void);
  virtual                                ~InvalidObjectInterface(void);
  static InvalidObjectInterface&          Singleton(void);
};
/*
 * ProfileInterface
 */
class ProfileInterface
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                                    ProfileInterface(void);
  virtual                          ~ProfileInterface(void);
public:
  const ProfileInterfaceAttribute&  AttributeAtIndex(unsigned int index) const;
  const ProfileInterfaceAttribute&  AttributeWithID(const MString& id) const;
  unsigned                          AttributeCount(void) const;
  
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
private:
# ifdef __INTERFACES_CPP__
  typedef std::list<ProfileInterfaceAttribute*> ProfileAttributes; /* Attributes are mine */
# else
  typedef void ProfileAttributes;
# endif /* __INTERFACES_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  //const MapInterface*   m_interface; /* USELESS ? */
  ProfileAttributes*    m_attributes;
};
class MutableProfileInterface : public ProfileInterface
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
            MutableProfileInterface(void);
  virtual  ~MutableProfileInterface(void);
  void      AddAttribute(const SQLAttribute* attr, const IProfileAttributeValue& value);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __INTERFACES_H__ */
