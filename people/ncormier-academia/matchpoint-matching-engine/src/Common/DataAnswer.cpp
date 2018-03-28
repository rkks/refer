/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    DataAnswer.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "DataAnswer.h"
#include "Instances.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * DataAnswer
 */
DataAnswer::DataAnswer(void)
{
  // Members initilization
  m_objects = new MList<MutableObjectInstance*>();
}
DataAnswer::~DataAnswer(void)
{
  // Clean up
  for (unsigned i = 0, n = m_objects->Count(); i < n; i++)
    delete m_objects->ElemAtIndex(i);
  if (m_objects)  delete m_objects;
}

const ObjectInstance& DataAnswer::ObjectAtIndex(unsigned int index) const
{
  return *(m_objects->ElemAtIndex(index));
}

unsigned  DataAnswer::ObjectsCount(void) const
{
  return m_objects->Count();
}

/*
 * MutableDataAnswer
 */
MutableDataAnswer::MutableDataAnswer(void)
{
  /* Nothing */
}

MutableDataAnswer::~MutableDataAnswer(void)
{
  /* Nothing */
}

MutableObjectInstance&  MutableDataAnswer::AddNewObject(const ObjectInterface& oInterface)
{
  MutableObjectInstance* ret = new MutableObjectInstance(oInterface);
  m_objects->PushBack(ret);
  return *ret;
}
