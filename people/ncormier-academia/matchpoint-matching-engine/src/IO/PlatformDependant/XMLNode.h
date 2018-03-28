/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLNode.h
 * Desc    :    XML Node header
 * Author  :    mat
 ******************************************************************************/

#ifndef __XMLNODE_H__
# define __XMLNODE_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "XMLAttributes.h"
# include <vector>
# include <string>
# include <map>

#ifdef _WIN32
# include "WinXMLUtils.h"
# define strdup   _strdup
# define strndup  WinXMLUtils::strndup
#endif /* !_WIN32 */

using namespace std;
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class XMLNode
{
  typedef std::vector<XMLNode *> XMLNodeVector;
  typedef std::map<MString, XMLNodeVector *, strcomparer> XMLNodeMap;
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  XMLNode(MString *name);
  ~XMLNode();

  const char *GetName();

  void AddChild(XMLNode *child);
  bool ChildExists(const char *key);
  bool ChildExists(const MString &key);

  void AddAttributes(XMLAttributes *attr);

  XMLNode *GetChild(const char *name);
  XMLNode *GetChild(const MString &name);
  XMLNode *GetChild(const char *name, unsigned int idx);
  XMLNode *GetChild(const MString &name, unsigned int idx);
  int GetChildrenCount(const char *name);
  int GetChildrenCount(const MString &name);

  //const char *QueryValue(string &query, int pos = 0);

  const char *GetData();
  void SetData(const char *data, int size);
  void SetData(MString *data);

private:
  void FreeNode();
  void FreeChildren();
  void FreeAttributes();
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  const MString *m_name;
private:
  XMLNodeMap m_children;
  MString *m_data;
  XMLAttributes *m_attributes;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __XMLNODE_H__ */
