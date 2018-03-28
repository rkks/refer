/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UserRequest.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __USER_REQUEST_H__
# define __USER_REQUEST_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifdef __USER_REQUEST_CPP__
#  include <list>
# endif /* __USER_REQUEST_CPP__ */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MutableObjectInterface;
class ObjectInterface;
class MString;
class MutableMatchRules;
class MatchRules;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class UserRequest
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          UserRequest(void);
  virtual                ~UserRequest(void);
public:
  unsigned                AnswerLimit(void) const;
  unsigned                MinMatchPercentage(void) const;
  const ObjectInterface&  ObjectByID(const MString& id) const;
  const ObjectInterface&  ObjectAtIndex(unsigned index) const;
  unsigned                ObjectsCount(void) const;
  const MatchRules&       Rules(void) const;
  /***************************************************************************/
  /* Protected types definition                                              */
  /***************************************************************************/
protected:
# ifdef __USER_REQUEST_CPP__
  typedef std::list<MutableObjectInterface*> Objects;
# else
  typedef void Objects;
# endif /* __USER_REQUEST_CPP__ */
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  unsigned            m_answerLimit;
  unsigned            m_minMatchPercentage;
  Objects*            m_objects; /* Protect properties */
  MutableMatchRules*  m_matchRules;
};

class MutableUserRequest : public UserRequest
{
  /* Temporary, all sets should be in the ctor */
public:
              MutableUserRequest(void);
  virtual    ~MutableUserRequest(void);
  void        SetAnswerLimit(unsigned answerLimit);
  void        SetMinMatchPercentage(unsigned minMatchPercentage);
  void        AddAllocatedObjectIntefaces(MutableObjectInterface* obj); /* TODO: a changer l'allocation devrait etre fait a l interieur de l'objet ... */
  void        SetAllocatedMatchRules(MutableMatchRules* rules); /* TODO: a changer l'allocation devrait etre fait a l interieur de l'objet ... */
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __USER_REQUEST_H__ */
