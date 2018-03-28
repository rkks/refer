/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLSAXParser.cpp
 * Desc    :    XML SAX Parser Unix implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "LibXMLSAXParser.h"
#include "LibXMLAttributes.h"
#include "ISAXHandler.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

xmlSAXHandler LibXMLSAXParser::lxml2_hdls =
{
  NULL, /* internalSubsetSAXFunc internalSubset; */
  NULL, /* isStandaloneSAXFunc isStandalone; */
  NULL, /* hasInternalSubsetSAXFunc hasInternalSubset; */
  NULL, /* hasExternalSubsetSAXFunc hasExternalSubset; */
  NULL, /* resolveEntitySAXFunc resolveEntity; */
  NULL, /* getEntitySAXFunc getEntity; */
  NULL, /* entityDeclSAXFunc entityDecl; */
  NULL, /* notationDeclSAXFunc notationDecl; */
  NULL, /* attributeDeclSAXFunc attributeDecl; */
  NULL, /* elementDeclSAXFunc elementDecl; */
  NULL, /* unparsedEntityDeclSAXFunc unparsedEntityDecl; */
  NULL, /* setDocumentLocatorSAXFunc setDocumentLocator; */
  (startDocumentSAXFunc) startDocument, /* startDocumentSAXFunc startDocument; */
  (endDocumentSAXFunc) endDocument, /* endDocumentSAXFunc endDocument; */
  (startElementSAXFunc) startElement, /* startElementSAXFunc startElement; */
  (endElementSAXFunc) endElement, /* endElementSAXFunc endElement; */
  NULL, /* referenceSAXFunc reference; */
  (charactersSAXFunc) characters, /* charactersSAXFunc characters; */
  NULL, /* ignorableWhitespaceSAXFunc ignorableWhitespace; */
  NULL, /* processingInstructionSAXFunc processingInstruction; */
  NULL, /* commentSAXFunc comment; */
  NULL, /* warningSAXFunc warning; */
  NULL, /* errorSAXFunc error; */
  NULL, /* fatalErrorSAXFunc fatalError; */
  NULL, /* getParameterEntitySAXFunc getParameterEntity; */
  NULL, /* cdataBlockSAXFunc cdataBlock; */
  NULL, /* externalSubsetSAXFunc externalSubset; */
  0, /* unsigned int initialized; */
  NULL, /* void *_private; */
  NULL, /* startElementNsSAX2Func startElementNs; */
  NULL, /* endElementNsSAX2Func endElementNs; */
  NULL, /* xmlStructuredErrorFunc serror; */
};

LibXMLSAXParser::LibXMLSAXParser(const char *path, ISAXHandler &handl) : XMLSAXParser(path, handl)
{
  //xmlSAXUserParseFile(&lxml2_hdls, this, m_path);
  m_pCtx = xmlCreateFileParserCtxt(m_path);
  if (m_pCtx != NULL)
    {
      xmlFree(m_pCtx->sax);
      m_pCtx->userData = this;
      m_pCtx->sax = &LibXMLSAXParser::lxml2_hdls;
    }
}

LibXMLSAXParser::LibXMLSAXParser(const char *buffer, int size, ISAXHandler &handl) : XMLSAXParser(buffer, size, handl)
{
  m_pCtx = xmlCreateMemoryParserCtxt(buffer, size);
  if (m_pCtx != NULL)
    {
      xmlFree(m_pCtx->sax);
      m_pCtx->userData = this;
      m_pCtx->sax = &LibXMLSAXParser::lxml2_hdls;
    }
}

LibXMLSAXParser::~LibXMLSAXParser()
{
  if (m_pCtx != NULL)
    {
      m_pCtx->sax = NULL;
      xmlFreeParserCtxt(m_pCtx);
      m_pCtx = NULL;
    }
}

bool LibXMLSAXParser::Parse()
{
  return (xmlParseDocument(m_pCtx) == 0);
}

void LibXMLSAXParser::startDocument(LibXMLSAXParser *parser)
{
  parser->m_handl.startDocument(parser);
}

void LibXMLSAXParser::endDocument(LibXMLSAXParser *parser)
{
  parser->m_handl.endDocument(parser);
}

void LibXMLSAXParser::startElement(LibXMLSAXParser *parser, const char *name, const char **attrs)
{
  parser->m_handl.startElement(parser, name, LibXMLAttributes(attrs));
}

void LibXMLSAXParser::endElement(LibXMLSAXParser *parser, const char *name)
{
  parser->m_handl.endElement(parser, name);
}

void LibXMLSAXParser::characters(LibXMLSAXParser *parser, const char *chars, int len)
{
  parser->m_handl.characters(parser, chars, len);
}
