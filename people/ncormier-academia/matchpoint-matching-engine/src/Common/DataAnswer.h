/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    DataAnswer.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __DATA_ANSWER_H__
# define __DATA_ANSWER_H__

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
class MutableObjectInstance;
class ObjectInterface;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
/*
 * DataAnswer
 */
class DataAnswer
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                          DataAnswer(void);
  virtual                ~DataAnswer(void);
public:
  const ObjectInstance&   ObjectAtIndex(unsigned int index) const;
  unsigned                ObjectsCount(void) const;

  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MList<MutableObjectInstance*>*  m_objects;
};
/*
 * MutableDataAnswer
 */
class MutableDataAnswer : public DataAnswer
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                          MutableDataAnswer(void);
  virtual                ~MutableDataAnswer(void);
  MutableObjectInstance&  AddNewObject(const ObjectInterface& oInterface);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __DATA_ANSWER_H__ */
