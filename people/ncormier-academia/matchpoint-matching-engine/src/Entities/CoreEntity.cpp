/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    CoreEntity.cpp
 * Desc    :    Core implementation file
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#define __CORE_ENTITY_CPP__
#define ENTITY_IS_KIND_OF(e, k)       typeid(*e) == typeid(k)
#define ROUTE_END(t)                  m_availableRoutes[t->m_route].nbrSteps - 1
#define TASK_NEXT_STEP(t)             t->m_step + 1
#define TASK_ERROR_ENTITY_HANDLER(t)  m_availableRoutes[t->m_route].errorHandler
#define TASK_NEXT_ENTITY_TYPE(t)      m_availableRoutes[t->m_route].route[TASK_NEXT_STEP(t)]
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <typeinfo>
#include "CoreEntity.h"
#include "TaskFifo.h"
#include "Task.h"
#include "NetworkEntity.h"
#include "IOEntity.h"
#include "MatchEntity.h"
#include "DataEntity.h"
#include "MatchTask.h"
#include "NotificationTask.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Static variables                                                          */
/*****************************************************************************/
CoreEntity*               CoreEntity::s_coreInstance = NULL;
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
* Launch
* ------
* Launch the matchpoint core
*/
void CoreEntity::LaunchCore(void)
{
  if (s_coreInstance)
    return;
  s_coreInstance = new CoreEntity();
}

void CoreEntity::TreatTask(Task* task, const Entity* sender)
{
  s_coreInstance->m_sendersFifo->Add(sender->m_id); /* Add sender id to core senders fifo */
  s_coreInstance->AddTask(task); /* Add task to core tasks fifo */
}

Task* CoreEntity::AllocTaskWithRoute(const Entity* sender, eAvailableRoute route /* = DEFAULT_ROUTE */)
{
  switch (route)
  {
  case DEFAULT_ROUTE:
    {
      Task* tsk = new MatchTask();
      tsk->m_route = DEFAULT_ROUTE;
      tsk->m_ownerID = sender->m_id;
      return tsk;
    }
  case BROADCAST_NOTIFICATION_ROUTE:
    {
      Task* tsk = new NotificationTask();
      tsk->m_route = BROADCAST_NOTIFICATION_ROUTE;
      tsk->m_ownerID = sender->m_id;      
      return tsk;
    }
  default:
    FASSERT(false, "Unknown route");
  }
  return NULL;
}

/*
* CoreEntity
* ----------
* CoreEntity is unique and always have an ID
* equal to zero.
* Only launch user entity
*/
CoreEntity::CoreEntity(void) : Entity(0)
{
  // Members initialization
  m_lastRegisteredID = 0;
  // Members creation
  m_sendersFifo = new TaskSendersFifo();
  m_lookupTable = new EntityLookupTable();
  m_ioEntities = new Entities(); 
  m_matchEntities =  new Entities();
  m_dataEntities = new Entities();
  m_toDeleteTasks = new Tasks();
  // Launch user entity
  // Other entities will be launch after
  m_networkEntity = dynamic_cast<NetworkEntity*>(newEntity(NETWORK_ENTITY));
  FASSERT(m_networkEntity ? true : false, "newEntity failed");
  // Build routes
  sRoute defaultRoute = 
  {
    {  
      NETWORK_ENTITY, /* Launch by network entity to io entity*/
      IO_ENTITY, /* ... */
      DATA_ENTITY,
      MATCH_ENTITY,
      IO_ENTITY,
      NETWORK_ENTITY
    },
    NETWORK_ENTITY, /* Default task failed handler for default route */
    6
  };
  m_availableRoutes[DEFAULT_ROUTE] = defaultRoute; /* Add to available routes */
  sRoute broadcastNotificationRoute =
  {
    {
      UNDEF_ENTITY, /* Launch by an undef entity to all entities*/
      ALL_ENTITIES  
    },
    UNDEF_ENTITY, /* Default task failed handler for broadcast route */
    2
   };
   m_availableRoutes[BROADCAST_NOTIFICATION_ROUTE] = broadcastNotificationRoute; /* Add to available routes */
   Launch(); /* Launch entity ! */
}

CoreEntity::~CoreEntity(void)
{
  if (m_networkEntity) delete m_networkEntity;
  // Delete user entities
  for (Entities::iterator iter = m_ioEntities->begin(); 
       iter != m_ioEntities->end(); iter++)
        delete *iter;
  // Delete matching entities
  for (Entities::iterator iter = m_matchEntities->begin(); 
       iter != m_matchEntities->end(); iter++)
        delete *iter;
  // Delete data entities
  for (Entities::iterator iter = m_dataEntities->begin(); 
       iter != m_dataEntities->end(); iter++)
        delete *iter;
  // Members deletion
  if (m_sendersFifo)    delete m_sendersFifo;
  if (m_lookupTable)    delete m_lookupTable;
  if (m_ioEntities)     delete m_ioEntities;
  if (m_matchEntities)  delete m_matchEntities;
  if (m_dataEntities)   delete m_dataEntities;
  if (m_toDeleteTasks)  delete m_toDeleteTasks;
}

const char* CoreEntity::Description(void)
{
  return "Core";
}

CoreEntity::eEntryPointStatus  CoreEntity::entryPoint(void)
{
  while (true)
  {
    Task* tsk = m_fifo->GetATask(); /* Wait for a new task */
    // Get sender id
    unsigned senderID;
    bool getRet = m_sendersFifo->Get(senderID);
    FASSERT(getRet, "Empty senders fifo");
    (void) ProcessTask(tsk, senderID); /* Process task 
                                          (CoreEntity process should never failed) */
    deleteUselessTasks(); /* Cleanup unused tasks */
  }
  return EXIT_SUCCESS;
}

eStatus  CoreEntity::ProcessTask(Task* tsk, unsigned senderID)
{
  if (senderID != tsk->m_ownerID)
    return PROCESS_SUCCEEDED; /* Not owner, skip route */
  Entities recipients;
  route(tsk, &recipients); /* Get task candidate(s) */
  if (recipients.size())
  {
    // Deliver the task
    if (recipients.size() == 1)
      tsk->m_ownerID = (*recipients.begin())->m_id;
    else
      tsk->m_ownerID = (unsigned) -1; /* Multiple */
    tsk->m_step = TASK_NEXT_STEP(tsk);
    // Add task to each recipient
    for (Entities::const_iterator iter = recipients.begin(); 
          iter != recipients.end(); iter++)
      (*iter)->AddTask(tsk);
  }
  else /* Task to delete */
  {
    bool bAlreadyMarkedDelete = false;
    // Check if task is already in delete queue
    for (Tasks::iterator iter = m_toDeleteTasks->begin();
      iter != m_toDeleteTasks->end(); iter++)
      if (*iter == tsk) bAlreadyMarkedDelete = true;
    if (!bAlreadyMarkedDelete) /* Not in delete queue */
    {
      if (!tsk->m_referenceCount) /* Check task count */
        delete tsk;
      else /* Add task to delete queue */
        m_toDeleteTasks->push_back(tsk);
    }
  }
  return PROCESS_SUCCEEDED;
}

void  CoreEntity::deleteUselessTasks(void)
{
  bool taskDeleted = false;
  do
  {
    for (Tasks::iterator iter = m_toDeleteTasks->begin();
          !taskDeleted && iter != m_toDeleteTasks->end(); iter++)
    {
      if (!(*iter)->m_referenceCount) /* Delete task */
      {
        delete *iter;
        m_toDeleteTasks->remove(*iter);
        taskDeleted = true;
      }
    }
  }
  while (taskDeleted);
}

void  CoreEntity::route(Task* tsk, Entities* recipients)
{
  eEntityType recipientsType = UNDEF_ENTITY;
  if (tsk->m_status == PROCESS_FAILED) /* Task treatment has failed */
  {
    if (taskLastEntityType(tsk) != TASK_ERROR_ENTITY_HANDLER(tsk))
      recipientsType = TASK_ERROR_ENTITY_HANDLER(tsk);
    else return; /* Delete task */
  }
  else
  {
    if (tsk->m_step == ROUTE_END(tsk)) /* Task treatment has succeeded */
      return; /* Delete task */
    recipientsType = TASK_NEXT_ENTITY_TYPE(tsk);
  }
  switch (recipientsType)
  {
  case NETWORK_ENTITY:
    recipients->push_back(m_networkEntity);
    break;
  case IO_ENTITY:
    recipients->push_back(seekForLowChargerEntity(m_ioEntities, IO_ENTITY)); /* Deliver task to an UserEntity */
    break;
  case MATCH_ENTITY:
    recipients->push_back(seekForLowChargerEntity(m_matchEntities, MATCH_ENTITY)); /* Deliver task to an MatchEntity */
    break;
  case DATA_ENTITY:
    recipients->push_back(seekForLowChargerEntity(m_dataEntities, DATA_ENTITY)); /* Deliver task to an DataEntity */
    break;
  case ALL_ENTITIES:
    for (EntityLookupTable::iterator iter = m_lookupTable->begin();
          iter != m_lookupTable->end(); iter++)
      recipients->push_back((*iter).second); /* Add all entities */
    break;
  case UNDEF_ENTITY:
  default:
    FASSERT(false, "Unknown entity");
  }
}

Entity* CoreEntity::seekForLowChargerEntity(Entities* entities, eEntityType targetType)
{
  TODO;
  // TEMPORARY
  // TODO:  There is a low charged entity ?
  //        No -> Create a new entity ?
  if (!entities->size())
  {
    Entity* toAdd = NULL;
    switch (targetType)
    {
    case IO_ENTITY:
      toAdd = dynamic_cast<IOEntity*>(newEntity(IO_ENTITY));
      break;
    case MATCH_ENTITY:
      toAdd = dynamic_cast<MatchEntity*>(newEntity(MATCH_ENTITY));
      break;
    case DATA_ENTITY:
      toAdd = dynamic_cast<DataEntity*>(newEntity(DATA_ENTITY));
      break;
    }
    FASSERT(toAdd ? true : false, "newEntity failed");
    entities->push_back(toAdd); /* Add to list */
  }
  Entities::const_iterator iter = entities->begin();
  return *iter;
}

/*
* entityLookup
* ------------
* Useless ?
*/
Entity* CoreEntity::entityLookup(unsigned id)
{
  EntityLookupTable::const_iterator iter = m_lookupTable->find(id);
  FASSERT(iter != m_lookupTable->end(), "Entity lookup failed, unknown id");
  return (*iter).second;
}

Entity* CoreEntity::newEntity(eEntityType targetType)
{
  Entity* ret = NULL;
  // Create object
  switch (targetType)
  {
  case NETWORK_ENTITY:
    ret = new NetworkEntity(++m_lastRegisteredID);
    break;
  case IO_ENTITY:
    ret = new IOEntity(++m_lastRegisteredID);
    break;
  case MATCH_ENTITY:
    ret = new MatchEntity(++m_lastRegisteredID);
    break;
  case DATA_ENTITY:
    ret = new DataEntity(++m_lastRegisteredID);
    break;
  default:
    FASSERT(false, "Unknown entity");
  }
  (*m_lookupTable)[m_lastRegisteredID] = ret; /* Register entity */
  return ret;
}

CoreEntity::eEntityType CoreEntity::taskLastEntityType(Task* tsk)
{
  Entity* entity = entityLookup(tsk->m_ownerID);
  if (ENTITY_IS_KIND_OF(entity, NetworkEntity))
    return NETWORK_ENTITY;
  if (ENTITY_IS_KIND_OF(entity, IOEntity))
    return IO_ENTITY;
  if (ENTITY_IS_KIND_OF(entity, MatchEntity))
    return MATCH_ENTITY;
  if (ENTITY_IS_KIND_OF(entity, DataEntity))
    return DATA_ENTITY;
  FASSERT(false, "Unknown entity");
  return UNDEF_ENTITY;
}
