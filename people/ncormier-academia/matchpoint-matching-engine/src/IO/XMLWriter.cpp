/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include "XMLWriter.h"
#include "MString.h"

DNSPACE


#define INDENT "  "

XMLWriter::XMLWriter(MString& output)
{
  m_level = 0;
  m_output= &output;
}

XMLWriter::~XMLWriter(void)
{
}

// OpenNode
// OpenNodeWithAttributes
// CloseNode
// AddAttribute
// CloseAttributes
// AddCData
// AddSimpleNode

void XMLWriter::internalWrite(const MString &str)
{
  *m_output += str;
}

void	XMLWriter::OpenNode(const MString &node)
{
  indent();
  internalWrite("<");
  internalWrite(node);
  internalWrite(">\r\n");
  m_level++;
}

void	XMLWriter::OpenNodeWithCDATA(const MString &node)
{
  indent();
  internalWrite("<");
  internalWrite(node);
  internalWrite(">");
}

void	XMLWriter::OpenNodeWithAttributes(const MString &node)
{
  indent();
  internalWrite("<");
  internalWrite(node);
}

void	XMLWriter::CloseNode(const MString &node)
{
  if (m_level > 0)
    m_level--;
  indent();
  internalWrite("</");
  internalWrite(node);
  internalWrite(">\r\n");
}

void	XMLWriter::CloseNodeWithCDATA(const MString &node)
{
  internalWrite("</");
  internalWrite(node);
  internalWrite(">\r\n");
}

void	XMLWriter::AddAttribute(const MString &key, const MString &value)
{
  internalWrite(" ");
  internalWrite(key);
  internalWrite("=");
  internalWrite("\"");
  internalWrite(value);
  internalWrite("\"");
}

void	XMLWriter::CloseAttributes(void)
{
  internalWrite(">\r\n");
  m_level++;
}

void	XMLWriter::AddCData(const MString &data)
{
  internalWrite(data);
}

void	XMLWriter::AddSimpleNode(const MString &node)
{
  indent();
  internalWrite("/>\r\n");
  if (m_level > 0)
    m_level--;
}

void	XMLWriter::indent(void)
{
  for (int i = 0; i < m_level; i++)
    internalWrite(INDENT);
}
