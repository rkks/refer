/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLNode.cpp
 * Desc    :    XML Node implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "XMLNode.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
XMLNode::XMLNode(MString *name) : m_name(name),
                            	  m_data(NULL),
                                  m_attributes(NULL)
{
}

XMLNode::~XMLNode()
{
  FreeNode();
  FreeAttributes();
}

const char *XMLNode::GetName()
{
  return (this->m_name->CharArray());
}

void XMLNode::AddChild(XMLNode *child)
{
  if (ChildExists(child->GetName()) == false)
    {
      m_children[MString(child->GetName())] = new XMLNodeVector;
    }
  (*m_children[MString(child->GetName())]).push_back(child);
}

bool XMLNode::ChildExists(const char *key)
{
  return (ChildExists(MString(key)));
}

bool XMLNode::ChildExists(const MString &key)
{
  if (m_children.find(key) != m_children.end())
    return (true);
  return (false);
}

void XMLNode::AddAttributes(XMLAttributes *attr)
{
  m_attributes = attr;
}

XMLNode *XMLNode::GetChild(const MString &name)
{
  if (ChildExists(name) == true)
    {
      return ((*m_children[name])[0]);
    }
  return (NULL);
}

XMLNode *XMLNode::GetChild(const char *name)
{
  return (GetChild(MString(name)));
}

XMLNode *XMLNode::GetChild(const MString &name, unsigned int idx)
{
  if (ChildExists(name) == true)
    if ((*m_children[name]).size() > idx)
    {
      return ((*m_children[name])[idx]);
    }
  return (NULL);
}

XMLNode *XMLNode::GetChild(const char *name, unsigned int idx)
{
  return (GetChild(MString(name), idx));
}

int XMLNode::GetChildrenCount(const MString &name)
{
  if (ChildExists(name) == true)
    {
      return ((int) (*m_children[name]).size());
    }
  return (0);
}

int XMLNode::GetChildrenCount(const char *name)
{
  return (GetChildrenCount(MString(name)));
}

const char *XMLNode::GetData()
{
  if (m_data != NULL)
    return (m_data->CharArray());
  return (NULL);
}

void XMLNode::SetData(MString *data)
{
  if (m_data != NULL)
    {
      delete m_data;
    }
  m_data = data;
}

void XMLNode::SetData(const char *data, int size)
{
  SetData(new MString(data, size));
}

void XMLNode::FreeNode()
{
  if (m_name != NULL)
  {
    delete m_name;
    m_name = NULL;
  }
  if (m_data != NULL)
  {
    delete m_data;
    m_data = NULL;
  }
  FreeChildren();
  FreeAttributes();
}

void XMLNode::FreeChildren()
{
  if (m_children.size() != 0)
  {
    XMLNodeMap::const_iterator iter_m = m_children.begin();
    while (iter_m != m_children.end())
    {

      XMLNodeVector::const_iterator iter_v = iter_m->second->begin();
      while (iter_v != iter_m->second->end())
      {
        delete *iter_v;
        iter_v++;
      }
      iter_m->second->clear();
      delete iter_m->second;
      iter_m++;
    }
    m_children.clear();
  }
}

void XMLNode::FreeAttributes()
{
  if (m_attributes != NULL)
    {
      delete m_attributes;
      m_attributes = NULL;
    }
}

//const char *XMLNode::QueryValue(string &query, int pos)
//{
//  if (query.size() == 0)
//    return (NULL);
//  int npos = (int) query.find("/", pos);
//  if (npos == -1)
//    {
//      return (this->m_attributes->GetValue(MString(query.c_str())));
//    }
//  const string &elem = query.substr(0, npos);
//  query = query.substr(npos + 1, query.size() - npos - 1);
//  if (ChildExists(elem.c_str()) == false)
//    return (NULL);
//  return (GetChild(elem.c_str(), 0)->QueryValue(query));
//}
