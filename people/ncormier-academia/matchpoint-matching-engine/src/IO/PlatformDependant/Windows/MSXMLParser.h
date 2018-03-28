/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MSXMLParser.h
 * Desc    :    XML Parser unix header
 * Author  :    mat
 ******************************************************************************/

#ifndef __MSXMLPARSER_H__
# define __MSXMLPARSER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#define assert(_Expression)     ((void)0)

# include <windows.h>
# import <msxml4.dll> raw_interfaces_only
# include "XMLParser.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"

using namespace MSXML2;

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class MSXMLParser : public XMLParser, public ISAXContentHandler
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  MSXMLParser(const char *path);
  MSXMLParser(const char *buffer, int size);
  ~MSXMLParser(void);
private:
  void Initialize();

  bool Parse();
  bool ParseFile();
  bool ParseMemory();

  long __stdcall QueryInterface(const struct _GUID &, void **);
  unsigned long __stdcall AddRef();
  unsigned long __stdcall Release();

  HRESULT STDMETHODCALLTYPE putDocumentLocator(ISAXLocator __RPC_FAR *pLocator);

  HRESULT STDMETHODCALLTYPE startDocument();

  HRESULT STDMETHODCALLTYPE endDocument();

  HRESULT STDMETHODCALLTYPE startPrefixMapping(wchar_t *pwchPrefix, int cchPrefix,
                                               wchar_t *pwchUri, int cchUri);

  HRESULT STDMETHODCALLTYPE endPrefixMapping(wchar_t *pwchPrefix, int cchPrefix);

  HRESULT STDMETHODCALLTYPE startElement(wchar_t *pwchNamespaceUri, int cchNamespaceUri,
                                         wchar_t *pwchLocalName, int cchLocalName,
                                         wchar_t *pwchRawName, int cchRawName,
                                         ISAXAttributes __RPC_FAR *pAttributes);

  HRESULT STDMETHODCALLTYPE endElement(wchar_t *pwchNamespaceUri,
                                       int cchNamespaceUri, wchar_t *pwchLocalName,
                                       int cchLocalName, wchar_t *pwchRawName, int cchRawName);

  HRESULT STDMETHODCALLTYPE characters(wchar_t *pwchChars, int cchChars);

  HRESULT STDMETHODCALLTYPE ignorableWhitespace(wchar_t *pwchChars, int cchChars);

  HRESULT STDMETHODCALLTYPE processingInstruction(wchar_t *pwchTarget, int cchTarget,
                                                  wchar_t *pwchData, int cchData);

  HRESULT STDMETHODCALLTYPE skippedEntity(wchar_t *pwchName, int cchName);

  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
private:
  ISAXXMLReader*  m_pReader;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __MSXMLPARSER_H__ */
