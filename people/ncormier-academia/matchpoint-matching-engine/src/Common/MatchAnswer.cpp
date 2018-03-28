/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MatchAnswer.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MatchAnswer.h"
#include "Instances.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * MatchAnswer
 */
MatchAnswer::MatchAnswer(void)
{
  // Members initilization
  m_objects = new MList<MutableMatchedObjectInstance*>();
}
MatchAnswer::~MatchAnswer(void)
{
  // Clean up
  for (unsigned i = 0, n = m_objects->Count(); i < n; i++)
    delete m_objects->ElemAtIndex(i);
  if (m_objects)  delete m_objects;
}

const MatchedObjectInstance& MatchAnswer::ObjectAtIndex(unsigned int index) const
{
  return *(m_objects->ElemAtIndex(index));
}

unsigned  MatchAnswer::ObjectsCount(void) const
{
  return m_objects->Count();
}

/*
 * MutableDataAnswer
 */
MutableMatchAnswer::MutableMatchAnswer(void)
{
  /* Nothing */
}

MutableMatchAnswer::~MutableMatchAnswer(void)
{
  /* Nothing */
}

MutableMatchedObjectInstance&  MutableMatchAnswer::AddNewObject(const ObjectInstance& instance)
{
  MutableMatchedObjectInstance* ret = new MutableMatchedObjectInstance(instance);
  m_objects->PushBack(ret);
  return *ret;
}
