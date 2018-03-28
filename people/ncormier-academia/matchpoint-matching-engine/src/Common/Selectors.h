/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Selectors.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __SELECTORS_H__
# define __SELECTORS_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class SQLAttribute;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
 * ISelector
 */
class ISelector
{
  /***************************************************************************/
  /* Public types definition                                                */
  /***************************************************************************/
public:
  /*
  * eSelectorType
  */
  enum _eSelectorType
  {
    NONE = 0,
    BELONGS_TO,
    HAS_MANY,
    HAS_AND_BELONGS_TO_MANY
  };
  typedef enum _eSelectorType eSelectorType;
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                  ISelector(void) {};
  virtual                        ~ISelector(void) {};
  virtual eSelectorType           GetType(void) const = 0;
};
/*
 * NoneSelector
 */
class NoneSelector : public ISelector
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                          NoneSelector(void);
  virtual                ~NoneSelector(void);
  eSelectorType           GetType(void) const;
};
/*
 * BelongsToSelector
 */
class BelongsToSelector : public ISelector
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                          BelongsToSelector(const SQLAttribute* local,
                                            const SQLAttribute* target);
  virtual                ~BelongsToSelector(void);
  eSelectorType           GetType(void) const;
  const SQLAttribute&     GetLocal(void) const;
  const SQLAttribute&     GetTarget(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*  m_local;
  const SQLAttribute*  m_target;
};
/*
 * HasManySelector
 */
class HasManySelector : public ISelector
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                          HasManySelector(const SQLAttribute* local,
                                          const SQLAttribute* target);
  virtual                ~HasManySelector(void);
  eSelectorType           GetType(void) const;
  const SQLAttribute&     GetLocal(void) const;
  const SQLAttribute&     GetTarget(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*     m_local;
  const SQLAttribute*     m_target;
};
/*
 * HABTMSelector
 */
class HABTMSelector : public ISelector
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                          HABTMSelector(const SQLAttribute* local,
                                        const SQLAttribute* localToInter,
                                        const SQLAttribute* interToTarget,
                                        const SQLAttribute* target);
  virtual                ~HABTMSelector(void);
  eSelectorType           GetType(void) const;
  const SQLAttribute&     GetLocal(void) const;
  const SQLAttribute&     GetLocalToInter(void) const;
  const SQLAttribute&     GetInterToTarget(void) const;
  const SQLAttribute&     GetTarget(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const SQLAttribute*  m_local;
  const SQLAttribute*  m_localToInter;
  const SQLAttribute*  m_interToTarget;
  const SQLAttribute*  m_target;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __SELECTORS_H__ */
