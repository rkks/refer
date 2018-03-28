/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    MSXMLParser.cpp
* Desc    :    XML Parser Windows implementation
* Author  :    mat
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MSXMLParser.h"
#include "MSXMLAttributes.h"
#include "MSXMLErrorHandler.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MSXMLParser::MSXMLParser(const char *path) : XMLParser(path)
{
  Initialize();
}

MSXMLParser::MSXMLParser(const char *buffer, int size) : XMLParser(buffer, size)
{
  Initialize();
}

MSXMLParser::~MSXMLParser(void)
{
  if (m_pReader)
    m_pReader->Release();
  m_pReader = NULL;

  if (m_errorHandl != NULL)
  {
    delete m_errorHandl;
    m_errorHandl = NULL;
  }

  CoUninitialize();
}

void MSXMLParser::Initialize()
{
  CoInitialize(NULL);
  m_pReader = NULL;

  HRESULT hr = CoCreateInstance(__uuidof(SAXXMLReader), NULL, CLSCTX_ALL, __uuidof(ISAXXMLReader), (void **)&m_pReader);
  if(FAILED(hr))
	  throw;

  hr = m_pReader->putContentHandler(this);
  if(FAILED(hr))
	  throw;

  m_errorHandl = new MSXMLErrorHandler();
  hr = m_pReader->putErrorHandler((MSXMLErrorHandler *) m_errorHandl);
  if(FAILED(hr))
	  throw;
}

bool MSXMLParser::Parse()
{
  if (!m_pReader)
    return false;

  if (m_path != NULL)
    return (ParseFile());
  return (ParseMemory());
}

bool MSXMLParser::ParseFile()
{
  wchar_t URL[1000];

  HRESULT hr = mbstowcs_s(NULL, (wchar_t *) URL, 999, m_path, 999);
  if(hr == S_FALSE || hr == S_OK)
  {
    hr = m_pReader->parseURL(URL);
  }
  return (!FAILED(hr));
}

bool MSXMLParser::ParseMemory()
{
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
  return (!FAILED(hr));
}

HRESULT STDMETHODCALLTYPE MSXMLParser::putDocumentLocator(ISAXLocator __RPC_FAR *pLocator)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::startDocument()
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::endDocument()
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::startPrefixMapping(wchar_t *pwchPrefix, int cchPrefix,
                                                          wchar_t *pwchUri, int cchUri)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::endPrefixMapping(wchar_t *pwchPrefix, int cchPrefix)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::startElement(wchar_t *pwchNamespaceUri, int cchNamespaceUri,
                                                    wchar_t *pwchLocalName, int cchLocalName,
                                                    wchar_t *pwchRawName, int cchRawName,
                                                    ISAXAttributes __RPC_FAR *pAttributes)
{
  XMLNode *node = this->OpenNode(WinXMLUtils::WideCharToMString(pwchLocalName, cchLocalName));
  if (pAttributes != NULL)
  {
    node->AddAttributes(new MSXMLAttributes(pAttributes));
  }
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::endElement(wchar_t *pwchNamespaceUri,
                                                  int cchNamespaceUri, wchar_t *pwchLocalName,
                                                  int cchLocalName, wchar_t *pwchRawName, int cchRawName)
{
  this->CloseNode(NULL);//modif pour check erreur
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::characters(wchar_t *pwchChars, int cchChars)
{
  this->GetCurrentNode()->SetData(WinXMLUtils::WideCharToMStringAlloc(pwchChars, cchChars));
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::ignorableWhitespace(wchar_t *pwchChars, int cchChars)
{
  return S_OK;
}


HRESULT STDMETHODCALLTYPE MSXMLParser::processingInstruction(wchar_t *pwchTarget, int cchTarget,
                                                             wchar_t *pwchData, int cchData)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLParser::skippedEntity(wchar_t *pwchVal, int cchVal)
{
  return S_OK;
}

long __stdcall MSXMLParser::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
  return 0;
}

unsigned long __stdcall MSXMLParser::AddRef()
{
  return 0;
}

unsigned long __stdcall MSXMLParser::Release()
{
  return 0;
}
