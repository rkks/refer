/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MSXMLSAXParser.cpp
 * Desc    :    XML Parser Windows implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MSXMLSAXParser.h"
#include "ISAXHandler.h"
#include "MSXMLAttributes.h"
#include "WinXMLUtils.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MSXMLSAXParser::MSXMLSAXParser(const char *path, ISAXHandler &handl) : XMLSAXParser(path, handl)
{
  CoInitialize(NULL);
  m_pReader = NULL;
  HRESULT hr = CoCreateInstance(__uuidof(SAXXMLReader), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&m_pReader);

  if(!FAILED(hr))
  {
    hr = m_pReader->putContentHandler(this);
  }
  else
  {
    throw;
  }
}

MSXMLSAXParser::MSXMLSAXParser(const char *buffer, int size, ISAXHandler &handl) : XMLSAXParser(buffer, size, handl)
{
}

MSXMLSAXParser::~MSXMLSAXParser(void)
{
  if (m_pReader)
    m_pReader->Release();
  m_pReader = NULL;

  CoUninitialize();
}

bool MSXMLSAXParser::Parse()
{
  if (!m_pReader)
    return false;

  wchar_t *buf = WinXMLUtils::CharToWideChar(m_buffer, m_bufsize);
  HRESULT hr;
  if (buf)
  {
    tagVARIANT st;
    st.vt = VT_BSTR;
    st.bstrVal = buf;
    hr = m_pReader->parse(st);
  }
  //free(buf); <--- ne pas oublier
  return !FAILED(hr);
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::putDocumentLocator(ISAXLocator __RPC_FAR *pLocator)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::startDocument()
{
  this->m_handl.startDocument(this);
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::endDocument()
{
  this->m_handl.endDocument(this);
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::startPrefixMapping(wchar_t *pwchPrefix, int cchPrefix,
                                                          wchar_t *pwchUri, int cchUri)
{
  return S_OK;
}


HRESULT STDMETHODCALLTYPE MSXMLSAXParser::endPrefixMapping(wchar_t *pwchPrefix, int cchPrefix)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::startElement(wchar_t *pwchNamespaceUri, int cchNamespaceUri,
                                                       wchar_t *pwchLocalName, int cchLocalName,
                                                       wchar_t *pwchRawName, int cchRawName,
                                                       ISAXAttributes __RPC_FAR *pAttributes)
{
  char *name = strndup(pwchLocalName, cchLocalName);
  this->m_handl.startElement(this, name, MSXMLAttributes(pAttributes));
  free(name);
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::endElement(wchar_t *pwchNamespaceUri,
                                                     int cchNamespaceUri, wchar_t *pwchLocalName,
                                                     int cchLocalName, wchar_t *pwchRawName, int cchRawName)
{
  char *name = strndup(pwchLocalName, cchLocalName);
  this->m_handl.endElement(this, name);
  free(name);
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::characters(wchar_t *pwchChars, int cchChars)
{
  char *chars = strndup(pwchChars, cchChars);
  this->m_handl.characters(this, chars, cchChars);
  free(chars);
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::ignorableWhitespace(wchar_t *pwchChars, int cchChars)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::processingInstruction(wchar_t *pwchTarget, int cchTarget,
                                                                wchar_t *pwchData, int cchData)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLSAXParser::skippedEntity(wchar_t *pwchVal, int cchVal)
{
  return S_OK;
}

long __stdcall MSXMLSAXParser::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
  return 0;
}

unsigned long __stdcall MSXMLSAXParser::AddRef()
{
  return 0;
}

unsigned long __stdcall MSXMLSAXParser::Release()
{
  return 0;
}
