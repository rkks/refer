/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    CoreEntity.h
 * Desc    :    CoreEntity object declaration
 *              CoreEntity handles the launching of others entities,
 *              tasks deleveries, tasks deletion, and entities load balancing.
 * Author  :    nico
 *****************************************************************************/

#ifndef __CORE_ENTITY_H__
# define __CORE_ENTITY_H__

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
# define CORE_TREAT_TASK(t)             CoreEntity::TreatTask(t, this)
# define CORE_ALLOC_TASK()              CoreEntity::AllocTaskWithRoute(this)
# define CORE_ALLOC_TASK_WITH_ROUTE(r)  CoreEntity::AllocTaskWithRoute(this, r)
/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifdef __CORE_ENTITY_CPP__
#  include <list>
#  include <map>
#  include "Fifo.h"
# endif /* __CORE_ENTITY_CPP__ */
# include "Entity.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class CoreEntity : Entity
{
  /***************************************************************************/
  /* Public types definition                                                 */
  /***************************************************************************/
public:
  /*
   * eAvailableRoute
   */
  enum _eAvailableRoute
  {
    DEFAULT_ROUTE = 0,
    BROADCAST_NOTIFICATION_ROUTE,
    AVAILABLE_ROUTE_COUNT
  };
  typedef enum _eAvailableRoute eAvailableRoute;
  /***************************************************************************/
  /* Public methods                                                          */
  /***************************************************************************/
public:
  static void             LaunchCore(void);
  static void             TreatTask(Task* task, const Entity* sender);
  static Task*            AllocTaskWithRoute(const Entity* sender, eAvailableRoute route = DEFAULT_ROUTE);
  /***************************************************************************/
  /* Private types definition                                                */
  /***************************************************************************/
public:
  /*
   * eEntityType
   */
  enum _eEntityType
  {
    NETWORK_ENTITY = 0,
    IO_ENTITY,
    MATCH_ENTITY,
    DATA_ENTITY,
    UNDEF_ENTITY,
    ALL_ENTITIES
  };
  typedef enum _eEntityType eEntityType;
  /*
   * sRoute
   */
  struct _sRoute
  {
# define MAX_ROUTE_TAB  64
    eEntityType   route[MAX_ROUTE_TAB];
    eEntityType   errorHandler;
    unsigned      nbrSteps;
  };
  typedef struct _sRoute sRoute;
# ifdef __CORE_ENTITY_CPP__
  /*
   * Fifo, lists & maps
   */
    typedef Fifo<unsigned>              TaskSendersFifo; 
    typedef std::map<unsigned, Entity*> EntityLookupTable;
    typedef std::list<Task*>            Tasks;
    typedef std::list<Entity*>          Entities;
# else
  typedef void TaskSendersFifo, EntityLookupTable, Entities, Tasks;
# endif /* __CORE_ENTITY_CPP__ */
  /***************************************************************************/
  /* Private methods                                                         */
  /***************************************************************************/
private:
                          CoreEntity(void);
  virtual                ~CoreEntity(void);
  const char*             Description(void);
  eEntryPointStatus       entryPoint(void);
  eStatus                 ProcessTask(Task* tsk, unsigned senderID);
  void                    deleteUselessTasks(void);
  void                    route(Task* tsk, Entities* recipients);
  Entity*                 seekForLowChargerEntity(Entities* entities, 
                                                  eEntityType targetType);
  Entity*                 entityLookup(unsigned id);
  Entity*                 newEntity(eEntityType targetType);
  unsigned                routeEnd(Task* tsk); 
  eEntityType             taskLastEntityType(Task* tsk);
  /***************************************************************************/
  /* Private static variables                                                */
  /***************************************************************************/
private:
  static CoreEntity*  s_coreInstance;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
private:
  sRoute              m_availableRoutes[AVAILABLE_ROUTE_COUNT];
  TaskSendersFifo*    m_sendersFifo; /* Complement of task fifo, to retrieve task send */
  EntityLookupTable*  m_lookupTable; /* Table ID to Entity */
  Entity*             m_networkEntity; /* An unique network entity */
  Entities*           m_ioEntities; /* List of input/output entity */
  Entities*           m_matchEntities; /* List of match entity */
  Entities*           m_dataEntities; /* List of data entity */
  Tasks*              m_toDeleteTasks; /* List of task */
  unsigned            m_lastRegisteredID;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __CORE_ENTITY_H__ */
