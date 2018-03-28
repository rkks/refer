/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Selectors.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __SELECTORS_CPP__
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "Selectors.h"
#include "Attributes.h"
#include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 * NoneSelector
 */
NoneSelector::NoneSelector(void)
{
}

NoneSelector::~NoneSelector(void)
{
}

ISelector::eSelectorType NoneSelector::GetType(void) const
{
  return ISelector::NONE;
}

/*
 * BelongsToSelector
 */
BelongsToSelector::BelongsToSelector(const SQLAttribute* local, const SQLAttribute* target)
{
  // Initialization
  m_local = local;
  m_target = target;
}

BelongsToSelector::~BelongsToSelector(void)
{
  /* NOTHING IS MINE */
}

ISelector::eSelectorType BelongsToSelector::GetType(void) const
{
  return ISelector::BELONGS_TO;
}

const SQLAttribute& BelongsToSelector::GetLocal(void) const
{
  return *m_local;
}

const SQLAttribute& BelongsToSelector::GetTarget(void) const
{
  return *m_target;
}

/*
 * HasManySelector
 */
HasManySelector::HasManySelector(const SQLAttribute* local, const SQLAttribute* target)
{
  // Initialization
  m_local = local;
  m_target = target;
}

HasManySelector::~HasManySelector(void)
{
  /* NOTHING IS MINE */
}

ISelector::eSelectorType HasManySelector::GetType(void) const
{
  return ISelector::BELONGS_TO;
}

const SQLAttribute& HasManySelector::GetLocal(void) const
{
  return *m_local;
}

const SQLAttribute& HasManySelector::GetTarget(void) const
{
  return *m_target;
}

/*
 * HABTMSelector
 */
HABTMSelector::HABTMSelector(const SQLAttribute* local, const SQLAttribute* localToInter, 
                             const SQLAttribute* interToTarget, const SQLAttribute* target)
{
  // Initialization
  m_local = local;
  m_localToInter = localToInter;
  m_interToTarget = interToTarget;
  m_target = target;
}

HABTMSelector::~HABTMSelector(void)
{
  /* NOTHING IS MINE */
}

ISelector::eSelectorType HABTMSelector::GetType(void) const
{
  return ISelector::HAS_AND_BELONGS_TO_MANY;
}

const SQLAttribute& HABTMSelector::GetLocal(void) const
{
  return *m_local;
}

const SQLAttribute& HABTMSelector::GetLocalToInter(void) const
{
  return *m_localToInter;
}

const SQLAttribute& HABTMSelector::GetInterToTarget(void) const
{
  return *m_interToTarget;
}

const SQLAttribute& HABTMSelector::GetTarget(void) const
{
  return *m_target;
}
