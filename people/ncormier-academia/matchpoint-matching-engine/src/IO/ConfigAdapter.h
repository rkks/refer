/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    ConfigAdapter.h
 * Desc    :    Config adapter from XML (header)
 * Author  :    mat
 ******************************************************************************/

#ifndef __CONFIGADAPTER_H__
# define __CONFIGADAPTER_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "XMLParser.h"
# include "Config.h"
# include "Interfaces.h"
# include "XMLErrorHandler.h"

#ifdef _WIN32
# define strcasecmp _stricmp
#endif /* !_WIN32 */

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class ConfigAdapter
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  static MutableConfig *Create(const char *path);
  static MutableConfig *Create(const char *buffer, int size);
  MutableConfig *Create(XMLParser *xp);
private:
  void HandleError(XMLErrorHandler *error);
  void CheckRoot(XMLNode *root);
  void GetEngineConfig(XMLNode *engine);
  void GetDataSourceConfig(XMLNode *datasource);
  void GetCacheConfig(XMLNode *cache);
  void GetDataMapConfig(XMLNode *datamap);
  MutableMapInterface *CreateMapInterface(XMLNode *map);
  void CreateLocalAttribute(MutableMapInterface *newmap, XMLNode *mapnode);
  const AttributeValueType *GetAttributeValueType(const char *type);
  void ResolveSubEntities(MutableMapInterface **maps, XMLNode *datamap);
  void CreateSubEntity(MutableMapInterface **maps, int idx, XMLNode *subentity);
  /***************************************************************************/
  /* Members                                                                 */
  /***************************************************************************/
protected:
private:
  MutableConfig *m_conf;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __CONFIGADAPTER_H__ */
