/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLSAXParser.cpp
 * Desc    :    XML SAX Parser base implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# ifndef WIN32
#  include "LibXMLSAXParser.h"
# else
#  include "MSXMLSAXParser.h"
# endif

#include "ISAXHandler.h"
#include "XMLSAXParser.h"

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
XMLSAXParser::XMLSAXParser(const char *path, ISAXHandler &handl) : m_path(path), m_handl(handl)
{
}

XMLSAXParser::XMLSAXParser(const char *buffer, int size, ISAXHandler &handl) : m_path(buffer), m_bufsize(size), m_handl(handl)
{
}

XMLSAXParser::~XMLSAXParser()
{
}

XMLSAXParser *XMLSAXParser::Factory(const char *path, ISAXHandler &handl)
{
#ifndef WIN32
  return (new LibXMLSAXParser(path, handl));
#else
  return (new MSXMLSAXParser(path, handl));
#endif
}

XMLSAXParser *XMLSAXParser::Factory(const char *buffer, int size, ISAXHandler &handl)
{
#ifndef WIN32
  return (new LibXMLSAXParser(buffer, size, handl));
#else
  return (new MSXMLSAXParser(buffer, size, handl));
#endif
}
