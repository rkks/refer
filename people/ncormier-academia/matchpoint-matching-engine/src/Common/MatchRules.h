/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchRules.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __MATCH_RULES_H__
# define __MATCH_RULES_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "MList.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MString;
class MatchRule;
class ObjectInterface;
class ProfileInterface;
class MutableProfileInterface;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class MatchRules
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          MatchRules(void);
  virtual                ~MatchRules(void);
public:
  const MString&          Name(void) const;
  const ObjectInterface&  RequestOn(void) const;
  const ObjectInterface&  AnswerOn(void) const;
  const ProfileInterface& ProfileAtIndex(unsigned index) const;
  unsigned  ProfileCount(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MString*                         m_name;
  ObjectInterface*                 m_requestOn;
  ObjectInterface*                 m_answerOn;
  MList<MutableProfileInterface*>  *m_profileInterfaces;
};

class MutableMatchRules : public MatchRules
{
  /* Temporary, all sets should be in the ctor */
public:
              MutableMatchRules(void);
  virtual    ~MutableMatchRules(void);
  void        SetName(const MString& name);
  void        SetRequestOn(const ObjectInterface& requestOn);
  void        SetAnswerOn(const ObjectInterface& answerOn);
  void        AddAllocatedProfile(MutableProfileInterface* profile); /* TODO: a changer l'allocation devrait etre fait a l interieur de l'objet ... */
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __MATCH_RULES_H__ */
