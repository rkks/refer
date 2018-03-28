/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLParser.cpp
 * Desc    :    XML Parser base implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifndef _WIN32
#  include "LibXMLParser.h"
# else
#  include "MSXMLParser.h"
# endif

#include "XMLParser.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
XMLParser::XMLParser(const char *path) : m_path(path)
{
  m_rootNode = NULL;
  m_errorHandl = NULL;
  m_buffer = NULL;
  m_bufsize = 0;
}

XMLParser::XMLParser(const char *buffer, int size) : m_buffer(buffer), m_bufsize(size)
{
  m_rootNode = NULL;
  m_errorHandl = NULL;
  m_path = NULL;
}

XMLParser::~XMLParser()
{
	if (m_rootNode != NULL)
	{
		delete m_rootNode;
		m_rootNode = NULL;
	}
}

XMLParser *XMLParser::Factory(const char *path)
{
#ifndef _WIN32
  return (new LibXMLParser(path));
#else
  return (new MSXMLParser(path));
#endif
}

XMLParser *XMLParser::Factory(const char *buffer, int size)
{
#ifndef WIN32
  return (new LibXMLParser(buffer, size));
#else
  return (new MSXMLParser(buffer, size));
#endif
}

XMLNode *XMLParser::OpenNode(MString name)
{
  XMLNode *node = new XMLNode(new MString(name));
  if (m_rootNode == NULL)
    m_rootNode = node;
  if (m_nodes.size() > 0)
    m_nodes.top()->AddChild(node);
  m_nodes.push(node);
  return (node);
}

void XMLParser::CloseNode(const char *name)
{
  if (m_nodes.size() <= 0)
    {
      /*error*/
    }
  else
    {
      m_nodes.pop();
    }
}

const char *XMLParser::QueryValue(string &query)
{
//  if (m_rootNode == NULL)
    return (NULL);
//  return (m_rootNode->QueryValue(query));
}

XMLNode *XMLParser::GetRootNode()
{
  return (m_rootNode);
}

XMLNode *XMLParser::GetCurrentNode()
{
  return (m_nodes.top());
}

XMLErrorHandler *XMLParser::GetError()
{
  return (m_errorHandl);
}