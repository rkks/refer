/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLParser.h
 * Desc    :    XML Parser base header
 * Author  :    mat
 ******************************************************************************/

#ifndef __XMLPARSER_H__
# define __XMLPARSER_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "XMLNode.h"
# include "XMLErrorHandler.h"
# include <stack>

using namespace std;

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class XMLParser
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
  XMLParser(const char *path);
  XMLParser(const char *buffer, int size);
  XMLNode *OpenNode(MString name);
  void CloseNode(const char *name);
public:
  virtual ~XMLParser();
  static XMLParser *Factory(const char *path);
  static XMLParser *Factory(const char *buffer, int size);
  virtual bool Parse() = 0;
  const char *QueryValue(string &query);
  XMLNode *GetRootNode();
  XMLNode *GetCurrentNode();
  XMLErrorHandler *GetError();

private:
  /***************************************************************************/
  /* Members                                                                 */
  /***************************************************************************/
protected:
  const char *m_path;
  const char *m_buffer;
  int m_bufsize;
  XMLErrorHandler *m_errorHandl;
private:
  XMLNode *m_rootNode;
  std::stack<XMLNode *> m_nodes;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __XMLPARSER_H__ */
