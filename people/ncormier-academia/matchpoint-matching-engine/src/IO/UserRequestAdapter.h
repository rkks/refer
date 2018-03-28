/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    UserRequestAdapter.h
 * Desc    :    UserRequest adapter from XML (header)
 * Author  :    mat
 ******************************************************************************/

#ifndef __USERREQUESTADAPTER_H__
# define __USERREQUESTADAPTER_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "XMLParser.h"
# include "UserRequest.h"
# include "Interfaces.h"
# include "Config.h"
# include "Attributes.h"
# include "MatchRules.h"
# include "MList.h"

#ifdef _WIN32
# define strcasecmp _stricmp
#endif /* !_WIN32 */

#define MSTR std::string
#define CSTR(x) (x).c_str()

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class UserRequestAdapter
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  static MutableUserRequest *Create(const char *path);
  static MutableUserRequest *Create(const char *buffer, int size);
  MutableUserRequest *Create(XMLParser *xp);
private:
  void CheckRoot(XMLNode *root);
  void GetRequestBaseConf(XMLNode *root);
  void GetRequestEntities(XMLNode *entities);
  MutableObjectInterface *CreateObjectInterface(XMLNode *entity);
  void CreateObjectInterfaceAttributeSub(XMLNode *attr, MutableObjectInterface *obji, const MapInterface &mapi);
  void CreateObjectInterfaceAttribute(XMLNode *attr, MutableObjectInterface *obji, const MapInterface &mapi, const char *entityid);
  const AttributeValueType *GetAttributeType(XMLNode *type);
  void GetRequestMatchRules(XMLNode *matchrules);
  MutableMatchRules *CreateBaseMatchRules(XMLNode *matchrules);
  void CreateRequestMatchRule(MutableMatchRules *mrs, XMLNode *matchrule);
  void CreateProfileAttributeSub(MutableProfileInterface *profile, XMLNode *attr);
  void CreateProfileAttribute(MutableProfileInterface *profile, const MapInterface &mapi, XMLNode *attr);
  IProfileAttributeValue *CreateProfileAttributeValue(const SQLAttribute &sqlattr, XMLNode *attr);
  IProfileAttributeValue *CreateSingleProfileAttributeValue(const AttributeValueType &avtype, const char *value);
  IProfileAttributeValue *CreateEnumProfileAttributeValue(const AttributeValueType &avtype, XMLNode *enumnode);
  IProfileAttributeValue *CreateRangeProfileAttributeValue(const AttributeValueType &avtype, XMLNode *rangenode);
  IAttributeValue *CreateAttributeValue(const AttributeValueType &avtype, const char *value);

  /***************************************************************************/
  /* Members                                                                 */
  /***************************************************************************/
protected:
private:
  MutableUserRequest *m_ureq;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __USERREQUESTADAPTER_H__ */
