/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Fifo.h
 * Desc    :    Thread safe fifo
 * Author  :    nico
 *****************************************************************************/

#ifndef __FIFO_H__
# define __FIFO_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include <list>
# include "Lock.h"
# include "Factory.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
template<typename T> class Fifo
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  Fifo(void)
  {
    // Objects initialization
    m_lock = Factory::CreateLock();
  }

  virtual  ~Fifo(void)
  {
    // Objects destruction
    if (m_lock) delete m_lock;
  }

  bool      Get(T& buf)
  {
    bool ret = false;

    m_lock->Retain();
    {
      if (m_list.size())
      {
        buf = m_list.front();
        m_list.pop_front();
        ret = true;
      }
    }
    m_lock->Release();
    return ret;
  }

  bool      Add(const T& buf)
  {
    m_lock->Retain();
      m_list.push_back(buf);
    m_lock->Release();
    return true; /* Temporary, !use stl */
  }

  unsigned  Count(void)
  {
    m_lock->Retain();
      unsigned ret = (unsigned) m_list.size();
    m_lock->Release();
    return ret;
  }
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  std::list<T>    m_list;
  Lock*           m_lock;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __FIFO_H__ */
