/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLParser.cpp
 * Desc    :    XML Parser Unix implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "LibXMLParser.h"
#include "LibXMLAttributes.h"
#include "LibXMLErrorHandler.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

xmlSAXHandler LibXMLParser::lxml2_hdls =
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
    (errorSAXFunc) error, /* errorSAXFunc error; */
    (fatalErrorSAXFunc) fatalError, /* fatalErrorSAXFunc fatalError; */
    NULL, /* getParameterEntitySAXFunc getParameterEntity; */
    NULL, /* cdataBlockSAXFunc cdataBlock; */
    NULL, /* externalSubsetSAXFunc externalSubset; */
    0, /* unsigned int initialized; */
    NULL, /* void *_private; */
    NULL, /* startElementNsSAX2Func startElementNs; */
    NULL, /* endElementNsSAX2Func endElementNs; */
    NULL, /* xmlStructuredErrorFunc serror; */
  };

LibXMLParser::LibXMLParser(const char *path) : XMLParser(path)
{

  //xmlSAXUserParseFile(&lxml2_hdls, this, m_path);
  m_pCtx = xmlCreateFileParserCtxt(m_path);
  if (m_pCtx != NULL)
    {
      xmlFree(m_pCtx->sax);
      m_pCtx->userData = this;
      m_pCtx->sax = &LibXMLParser::lxml2_hdls;
    }
  m_errorHandl = new LibXMLErrorHandler();
}

LibXMLParser::LibXMLParser(const char *buffer, int size) : XMLParser(buffer, size)
{
  m_pCtx = xmlCreateMemoryParserCtxt(buffer, size);
  if (m_pCtx != NULL)
    {
      xmlFree(m_pCtx->sax);
      m_pCtx->userData = this;
      m_pCtx->sax = &LibXMLParser::lxml2_hdls;
    }
  m_errorHandl = new LibXMLErrorHandler();
}

LibXMLParser::~LibXMLParser()
{
  if (m_pCtx != NULL)
    {
      m_pCtx->sax = NULL;
      xmlFreeParserCtxt(m_pCtx);
      m_pCtx = NULL;
    }

  if (m_errorHandl != NULL)
    {
      delete m_errorHandl;
      m_errorHandl = NULL;
    }
}

bool LibXMLParser::Parse()
{
  return (xmlParseDocument(m_pCtx) == 0);
}

void LibXMLParser::startDocument(LibXMLParser *parser)
{
}

void LibXMLParser::endDocument(LibXMLParser *parser)
{
}

void LibXMLParser::startElement(LibXMLParser *parser, const char *name, const char **attrs)
{
  XMLNode *node = parser->OpenNode(MString(name));
  if (attrs != NULL)
    {
      node->AddAttributes(new LibXMLAttributes(attrs));
    }
}

void LibXMLParser::endElement(LibXMLParser *parser, const char *name)
{
  parser->CloseNode(name);
}

void LibXMLParser::characters(LibXMLParser *parser, const char *chars, int len)
{
  parser->GetCurrentNode()->SetData(new MString(chars, len));
}

void LibXMLParser::error(LibXMLParser *parser, const char *msg, ...)
{
  va_list args;

  va_start(args, msg);
  char *errormsg = va_arg(args, char *);
  va_end(args);
  parser->m_errorHandl->SetErrorMessage(MString(errormsg));
  parser->m_errorHandl->SetErrorLine(parser->m_pCtx->input->line);
  parser->m_errorHandl->SetErrorColumn(parser->m_pCtx->input->col);
  xmlStopParser(parser->m_pCtx);
}

void LibXMLParser::fatalError(LibXMLParser *parser, const char *msg, ...)
{
  va_list args;

  va_start(args, msg);
  char *errormsg = va_arg(args, char *);
  va_end(args);
  parser->m_errorHandl->SetErrorMessage(MString(errormsg));
  parser->m_errorHandl->SetErrorLine(parser->m_pCtx->input->line);
  parser->m_errorHandl->SetErrorColumn(parser->m_pCtx->input->col);
  xmlStopParser(parser->m_pCtx);
}
