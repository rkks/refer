/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLSAXParser.h
 * Desc    :    XML Parser unix header
 * Author  :    mat
 ******************************************************************************/

#ifndef __LIBXMLSAXPARSER_H__
# define __LIBXMLSAXPARSER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "XMLSAXParser.h"
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

class LibXMLSAXParser : public XMLSAXParser
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  LibXMLSAXParser(const char *path, ISAXHandler &handl);
  LibXMLSAXParser(const char *buffer, int size, ISAXHandler &handl);
  ~LibXMLSAXParser();
private:
  bool Parse();
  static void startDocument(LibXMLSAXParser *parser);
  static void endDocument(LibXMLSAXParser *parser);
  static void startElement(LibXMLSAXParser *parser, const char *name, const char **attrs);
  static void endElement(LibXMLSAXParser *parser, const char *name);
  static void characters(LibXMLSAXParser *parser, const char *chars, int len);
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

#endif /* __LIBXMLSAXPARSER_H__ */
