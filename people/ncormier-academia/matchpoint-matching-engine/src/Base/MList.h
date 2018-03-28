/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MList.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __MLIST_H__
# define __MLIST_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <list>
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
template <typename T>
class MList
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  MList(void)
  {
    // Objects initialization
    m_list = new std::list<T>();
  }

  virtual ~MList(void)
  {
    // Objects cleanup
    if (m_list) delete m_list;
  }

  T ElemAtIndex(unsigned index) const
  {
    typename std::list<T>::const_iterator iter;
    for (iter = m_list->begin(); index && iter != m_list->end(); iter++, index--);
    FASSERT(iter != m_list->end(), "Out of range");
    return *iter;
  }

  const T ConstElemAtIndex(unsigned index) const
  {
    return ElemAtIndex(index);
  }

  unsigned  Count(void) const
  {
    return (unsigned) m_list->size();
  }

  void PushFront(T newElem)
  {
    m_list->push_front(newElem);
  }

  void PushBack(T newElem)
  {
    m_list->push_back(newElem);
  }
 
  void Clear(void)
  {
    m_list->clear();
  }
 
  void Remove(T elemToDelete)
  {
    m_list->remove(elemToDelete);
  }
  
  MList<T>* Clone(void)
  {
    MList<T>* ret = new MList<T>();
    for (unsigned i = 0, n = Count(); i < n; i++)
      ret->PushBack(ElemAtIndex(i));
    return ret;
  }
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  std::list<T>* m_list;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __STRING_H__ */
