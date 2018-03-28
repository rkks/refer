/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchAnswer.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __MATCH_ANSWER_H__
# define __MATCH_ANSWER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "MList.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class ObjectInstance;
class MutableMatchedObjectInstance;
class ObjectInterface;
class MatchedObjectInstance;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
 * MatchAnswer
 */
class MatchAnswer
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                                  MatchAnswer(void);
  virtual                        ~MatchAnswer(void);
public:
  const MatchedObjectInstance&    ObjectAtIndex(unsigned int index) const;
  unsigned                        ObjectsCount(void) const;

  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MList<MutableMatchedObjectInstance*>*  m_objects;
};
/*
 * MutableMatchAnswer
 */
class MutableMatchAnswer : public MatchAnswer
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                                  MutableMatchAnswer(void);
  virtual                        ~MutableMatchAnswer(void);
  MutableMatchedObjectInstance&   AddNewObject(const ObjectInstance& instance);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __MATCH_ANSWER_H__ */
