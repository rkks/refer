/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    ConfigAdapter.cpp
 * Desc    :    Config adapter from XML (implementation)
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <iostream>
#include "ConfigAdapter.h"
#include "MString.h"
#include "Attributes.h"
#include "Assert.h"
#include "Exception.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

MutableConfig *ConfigAdapter::Create(const char *path)
{
  ConfigAdapter adapter;

  XMLParser *xp = XMLParser::Factory(path);
  MutableConfig *conf = adapter.Create(xp);
  delete xp;
  return (conf);
}

MutableConfig *ConfigAdapter::Create(const char *buffer, int size)
{
  ConfigAdapter adapter;

  XMLParser *xp = XMLParser::Factory(buffer, size);
  MutableConfig *conf = adapter.Create(xp);
  delete xp;
  return (conf);
}

MutableConfig *ConfigAdapter::Create(XMLParser *xp)
{
  if (xp->Parse() != true)
    HandleError(xp->GetError());

  m_conf = new MutableConfig();
  XMLNode *curnode = xp->GetRootNode();

  CheckRoot(curnode);
  GetEngineConfig(curnode->GetChild("engine"));
  GetDataSourceConfig(curnode->GetChild("datasource"));
  GetCacheConfig(curnode->GetChild("cache"));
  GetDataMapConfig(curnode->GetChild("datamap"));

  return (m_conf);
}

void ConfigAdapter::HandleError(XMLErrorHandler *error)
{
  if (!error->IsErrorSet())
  {
    std::cerr << "Cannot access or load configuration file" << std::endl;
    FASSERT(false, "Cannot access or load configuration file");
  }
  else
  {
    std::cerr << "Error in the configuration XML file (line:";
    std::cerr << error->GetErrorLine();
    std::cerr << ", column:";
    std::cerr << error->GetErrorColumn();
    std::cerr << ")" << std::endl;
    std::cerr << error->GetErrorMessage() << std::endl;
    FASSERT(false, error->GetErrorMessage());
  }
}

void ConfigAdapter::CheckRoot(XMLNode *root)
{
  FASSERT(root != NULL, "root node does not exist");
}

void ConfigAdapter::GetEngineConfig(XMLNode *engine)
{
  FASSERT(engine != NULL, "engine node does not exist");
  FASSERT(engine->ChildExists("port") == true, "no tcp port available from configuration");

  int portnbr = engine->GetChildrenCount("port");
  XMLNode *portnode;
  for (int i = 0; i < portnbr; i++)
  {
    portnode = engine->GetChild("port", i);

    const char *portstr = portnode->GetData();
    FASSERT(portstr != NULL && strlen(portstr) != 0, "no tcp port specified");

    int port = atoi(portstr);
    FASSERT(port > 0 && port < 65535, "wrong tcp port number");

    m_conf->AddPort((unsigned short) port);
  }
}

void ConfigAdapter::GetDataSourceConfig(XMLNode *datasource)
{
  FASSERT(datasource != NULL, "data source is not specified");
  const char *type, *host, *user, *pass, *port, *db;

  if (datasource->ChildExists("type"))
  {
    type = datasource->GetChild("type")->GetData();
  }
  if (datasource->ChildExists("host"))
  {
    host = datasource->GetChild("host")->GetData();
  }
  if (datasource->ChildExists("username"))
  {
    user = datasource->GetChild("username")->GetData();
  }
  if (datasource->ChildExists("password"))
  {
    pass = datasource->GetChild("password")->GetData();
  }
  if (datasource->ChildExists("port"))
  {
    port = datasource->GetChild("port")->GetData();
  }
  if (datasource->ChildExists("db"))
  {
    db = datasource->GetChild("db")->GetData();
  }

  FASSERT(type != NULL, "BDD type not specified");
  FASSERT(host != NULL, "Host not specified");
  FASSERT(user != NULL, "User not specified");
  FASSERT(port != NULL, "Port not specified");
  FASSERT(db != NULL, "Port not specified");

  m_conf->SetBDDConfig(type, host, user, pass == NULL ? "" : pass, port, db);
}

void ConfigAdapter::GetCacheConfig(XMLNode *cache)
{
  if (cache != NULL)
    {
      FASSERT(cache->ChildExists("server") == true, "no tcp port available for cache configuration");
      FASSERT(cache->ChildExists("expire") == true, "no expiration time available for cache configuration");

      m_conf->SetCacheExpire(cache->GetChild("expire")->GetData());

      int srvnbr = cache->GetChildrenCount("server");
      XMLNode *srvnode;
      const char *ip, *port, *expire;
      for (int i = 0; i < srvnbr; i++)
	{
	  srvnode = cache->GetChild("server", i);

	  ip = port = expire = NULL;
	  if (srvnode->ChildExists("ip"))
	    {
	      ip = srvnode->GetChild("ip")->GetData();
	    }
	  if (srvnode->ChildExists("port"))
	    {
	      port = srvnode->GetChild("port")->GetData();
	    }

	  m_conf->AddMemcacheServer(ip, port);
	}
    }
}

void ConfigAdapter::GetDataMapConfig(XMLNode *datamap)
{
  FASSERT(datamap != NULL, "no data map specified");
  int entitymapnbr = datamap->GetChildrenCount("entitymap");
  FASSERT(entitymapnbr > 0, "you need to specify at least 1 entity map");

  MutableMapInterface **maps = new MutableMapInterface*[entitymapnbr + 1]();
  for (int i = 0; i < entitymapnbr; i++)
  {
    maps[i] = CreateMapInterface(datamap->GetChild("entitymap", i));
  }
  maps[entitymapnbr] = NULL;

  ResolveSubEntities(maps, datamap);

  delete[] maps;
}

MutableMapInterface *ConfigAdapter::CreateMapInterface(XMLNode *entitymap)
{
  FASSERT(entitymap != NULL, "wrong element where it should be 'entitymap'");
  FASSERT(entitymap->ChildExists("id") == true, "element 'id' is missing for 'entitymap'");
  FASSERT(entitymap->ChildExists("tablename") == true, "element 'tablename' is missing for 'entitymap'");
  FASSERT(entitymap->GetChildrenCount("map") > 0, "you need to specify at least one 'map' element for the 'entitymap'");

  MutableMapInterface *newmap = m_conf->AddMapInterface(entitymap->GetChild("id")->GetData(), entitymap->GetChild("tablename")->GetData());
  int localattrnbr = entitymap->GetChildrenCount("map");
  for (int i = 0; i < localattrnbr; i++)
  {
    CreateLocalAttribute(newmap, entitymap->GetChild("map", i));
  }
  return (newmap);
}

void ConfigAdapter::CreateLocalAttribute(MutableMapInterface *newmap, XMLNode *mapnode)
{
  FASSERT(mapnode != NULL, "wrong element where it should be 'map'");
  FASSERT(mapnode->ChildExists("id") == true, "element 'id' is missing for 'map'");
  FASSERT(mapnode->ChildExists("columnname") == true, "element 'columnname' is missing for 'map'");
  FASSERT(mapnode->ChildExists("type") == true, "element 'type' is missing for 'map'");

  newmap->AddLocalAttribute(MString(mapnode->GetChild("id")->GetData()), MString(mapnode->GetChild("columnname")->GetData()), GetAttributeValueType(mapnode->GetChild("type")->GetData()));
}

const AttributeValueType *ConfigAdapter::GetAttributeValueType(const char *type)
{
  FASSERT(type != NULL && strlen(type) != 0, "you need to specify a type for the map");

  if (!strcasecmp(type, "string"))
  {
    return (AttributeValueType::String());
  }
  else if (!strcasecmp(type, "int"))
  {
    return (AttributeValueType::Int());
  }
  else if (!strcasecmp(type, "float"))
  {
    return (AttributeValueType::Float());
  }
  else if (!strcasecmp(type, "list"))
  {
    return (AttributeValueType::List());
  }
  FASSERT(false, "unknown type for the map");
  return (NULL);
}

void ConfigAdapter::ResolveSubEntities(MutableMapInterface **maps, XMLNode *datamap)
{
  int entitymapnbr = datamap->GetChildrenCount("entitymap");
  for (int i = 0; i < entitymapnbr; i++)
  {
    XMLNode *entitymap = datamap->GetChild("entitymap", i);
    int subentitynbr = entitymap->GetChildrenCount("subentity");
    for (int j = 0; j < subentitynbr; j++)
    {
      CreateSubEntity(maps, i, entitymap->GetChild("subentity", j));
    }
  }
}

void ConfigAdapter::CreateSubEntity(MutableMapInterface **maps, int idx, XMLNode *subentity)
{
  FASSERT(subentity != NULL, "wrong element where it should be 'subentity'");
  FASSERT(subentity->ChildExists("entityid") == true, "element 'entityid' is missing for 'subentity'");
  FASSERT(subentity->ChildExists("linktype") == true, "element 'linktype' is missing for 'subentity'");
  FASSERT(subentity->ChildExists("localid") == true, "element 'localid' is missing for 'subentity'");
  FASSERT(subentity->ChildExists("targetid") == true, "element 'targetid' is missing for 'subentity'");

  MString entityid(subentity->GetChild("entityid")->GetData());
  for (int i = 0; maps[i] != NULL; i++)
  {
    if (entityid == maps[i]->MapID())
    {
      const char *type = subentity->GetChild("linktype")->GetData();
      if (strcasecmp(type, "belongsto") == 0)
      {
        maps[idx]->AddSubMapInterfaceWithBelongsToLink(maps[i],
          MString(subentity->GetChild("localid")->GetData()),
          MString(subentity->GetChild("targetid")->GetData()));
        maps[i]->AddSubMapInterfaceWithBelongsToLink(maps[idx],
          MString(subentity->GetChild("targetid")->GetData()),
          MString(subentity->GetChild("localid")->GetData()));
      }
      else if (strcasecmp(type, "habtm") == 0)
	    {
// 	      if (subentity->ChildExists("localtointercolumnname") == false || subentity->ChildExists("intertotargetcolumnname") == false)
// 		{
// 		  throw;
// 		}
// 	      maps[idx]->AddSubMapInterfaceWithHABTMLink(maps[i],
// 							     MString(subentity->GetChild("localcolumnname")->GetData()),
// 							     MString(subentity->GetChild("targetcolumnname")->GetData()));
	    }
	  else
	    {
	      FASSERT(false, "wrong type of interface (should be 'belongsto' or 'habtm')");
	    }
	  break;
	}
    }
}
