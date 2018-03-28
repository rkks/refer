/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    MSXMLErrorHandler.h
* Desc    :    XML Error Handler windows header
* Author  :    mat
******************************************************************************/

#ifndef __MSXMLERRORHANDLER_H__
# define __MSXMLERRORHANDLER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#define assert(_Expression)     ((void)0)

# include <windows.h>
# import <msxml4.dll> raw_interfaces_only
# include "XMLErrorHandler.h"
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

class MSXMLErrorHandler : public XMLErrorHandler, public ISAXErrorHandler
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  MSXMLErrorHandler();
  virtual ~MSXMLErrorHandler(void);
private:

 		long STDMETHODCALLTYPE QueryInterface(const struct _GUID &, void **);
		unsigned long STDMETHODCALLTYPE AddRef(void);
		unsigned long STDMETHODCALLTYPE Release(void);


  HRESULT STDMETHODCALLTYPE error(struct ISAXLocator *pLocator,
                                  wchar_t *pwchErrorMessage,
                                  HRESULT hrErrorCode);

  HRESULT STDMETHODCALLTYPE fatalError(struct ISAXLocator *pLocator,
                                        wchar_t *pwchErrorMessage,
                                        HRESULT hrErrorCode );

  HRESULT STDMETHODCALLTYPE ignorableWarning(struct ISAXLocator *pLocator,
                                              wchar_t *pwchErrorMessage,
                                              HRESULT hrErrorCode);

  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __MSXMLERRORHANDLER_H__ */
