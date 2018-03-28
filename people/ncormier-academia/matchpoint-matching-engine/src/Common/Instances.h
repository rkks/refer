/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Instances.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __INSTANCES_H__
# define __INSTANCES_H__

/* Header files                                                              */
/*****************************************************************************/
# ifdef __INSTANCES_CPP__
#  include <list>
# endif /* __INSTANCES_CPP__ */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MString;
class ObjectInterface;
class ObjectInstanceAttribute;
class IAttributeValue;
class SQLAttribute;
class MatchedObjectInstanceAttribute;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
 * ObjectInstance
 */
class ObjectInstance
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                                  ObjectInstance(void);
  virtual                        ~ObjectInstance(void);
public:
  const MString&                  ObjectID(void) const;
  const ObjectInstanceAttribute&  AttributeAtIndex(unsigned int index) const;
  const ObjectInstanceAttribute&  AttributeWithID(const MString& id) const;
  unsigned                        AttributeCount(void) const;
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
protected:
# ifdef __INSTANCES_CPP__
  typedef std::list<ObjectInstanceAttribute*> ObjectInstanceAttributes; /* Attributes are mine */ 
# else
  typedef void ObjectInstanceAttributes;
# endif /* __INSTANCES_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MString*                    m_id;
  ObjectInstanceAttributes*   m_attributes;
};
/*
 * MutableObjectInstance
 */
class MutableObjectInstance : public ObjectInstance
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                MutableObjectInstance(const ObjectInterface& obj);
  virtual      ~MutableObjectInstance(void);
  void          AddNewAttribute(const SQLAttribute* attr, const IAttributeValue& value);
};
/*
 * MatchedObjectInstance
 */
class MatchedObjectInstance
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                                          MatchedObjectInstance(void);
  virtual                                ~MatchedObjectInstance(void);
public:
  const MatchedObjectInstanceAttribute&   AttributeAtIndex(unsigned int index) const;
  const MatchedObjectInstanceAttribute&   AttributeWithID(const MString& id) const;
  unsigned                                AttributeCount(void) const;
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
protected:
# ifdef __INSTANCES_CPP__
  typedef std::list<MatchedObjectInstanceAttribute*> MatchedObjectInstanceAttributes; /* Attributes are mine */ 
# else
  typedef void MatchedObjectInstanceAttributes;
# endif /* __INSTANCES_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MatchedObjectInstanceAttributes*  m_attributes;
};
/*
 * MutableMatchedObjectInstance
 */
class MutableMatchedObjectInstance : public MatchedObjectInstance
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                MutableMatchedObjectInstance(const ObjectInstance& obj);
  virtual      ~MutableMatchedObjectInstance(void);
};

/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __INSTANCES_H__ */
