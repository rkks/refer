/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    MSXMLErrorHandler.cpp
* Desc    :    XML Error Handler Windows implementation
* Author  :    mat
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MSXMLErrorHandler.h"
#include "WinXMLUtils.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MSXMLErrorHandler::MSXMLErrorHandler() : XMLErrorHandler()
{
}

MSXMLErrorHandler::~MSXMLErrorHandler()
{
}

HRESULT STDMETHODCALLTYPE MSXMLErrorHandler::error(struct ISAXLocator *pLocator,
													wchar_t *pwchErrorMessage,
													HRESULT hrErrorCode)
{
  int length, line = 0, col = 0;
  length = (int) wcslen(pwchErrorMessage);
  SetErrorMessage(WinXMLUtils::WideCharToMString(pwchErrorMessage, length));
  if (pLocator != NULL)
  {
    pLocator->getLineNumber(&line);
    pLocator->getColumnNumber(&col);
    SetErrorLine(line);
    SetErrorColumn(col);
  }
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLErrorHandler::fatalError(struct ISAXLocator *pLocator,
														wchar_t *pwchErrorMessage,
														HRESULT hrErrorCode)
{
  int length, line = 0, col = 0;
  length = (int) wcslen(pwchErrorMessage);
  SetErrorMessage(WinXMLUtils::WideCharToMString(pwchErrorMessage, length));
  if (pLocator != NULL)
  {
    pLocator->getLineNumber(&line);
    pLocator->getColumnNumber(&col);
    SetErrorLine(line);
    SetErrorColumn(col);
  }
  return S_OK;
}

HRESULT STDMETHODCALLTYPE MSXMLErrorHandler::ignorableWarning(struct ISAXLocator *pLocator,
																wchar_t *pwchErrorMessage,
																HRESULT hrErrorCode)
{
  return S_OK;
}

long STDMETHODCALLTYPE MSXMLErrorHandler::QueryInterface(const struct _GUID &, void **)
{
	// only for compilation
	return S_OK;
}

unsigned long STDMETHODCALLTYPE MSXMLErrorHandler::AddRef()
{
	// only for compilation
	return S_OK;
}

unsigned long STDMETHODCALLTYPE MSXMLErrorHandler::Release()
{
	// only for compilation
	return S_OK;
}