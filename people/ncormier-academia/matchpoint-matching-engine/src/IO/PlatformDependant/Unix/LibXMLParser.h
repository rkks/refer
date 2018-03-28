/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLParser.h
 * Desc    :    XML Parser unix header
 * Author  :    mat
 ******************************************************************************/

#ifndef __LIBXMLPARSER_H__
# define __LIBXMLPARSER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "XMLParser.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class LibXMLParser : public XMLParser
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  LibXMLParser(const char *path);
  LibXMLParser(const char *buffer, int size);
  ~LibXMLParser();
private:
  bool Parse();
  static void startDocument(LibXMLParser *parser);
  static void endDocument(LibXMLParser *parser);
  static void startElement(LibXMLParser *parser, const char *name, const char **attrs);
  static void endElement(LibXMLParser *parser, const char *name);
  static void characters(LibXMLParser *parser, const char *chars, int len);
  static void error(LibXMLParser *parser, const char *msg, ...);
  static void fatalError(LibXMLParser *parser, const char *msg, ...);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
private:
  xmlParserCtxtPtr m_pCtx;
  static xmlSAXHandler lxml2_hdls;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __LIBXMLPARSER_H__ */
