/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    WinXMLUtils.h
 * Desc    :    XML Node header
 * Author  :    mat
 ******************************************************************************/

#ifndef __WINXMLUTILS_H__
# define __WINXMLUTILS_H__

# include "DefaultNamespace.h"
# include <string.h>
# include <stdlib.h>
# include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
#define MAXWCHARBUFSIZE 1024

class WinXMLUtils
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  static char *strndup(const char *src, int size);
  static char *strndup(wchar_t *src, int size);
  static void WideCharToMString(wchar_t *src, int size, MString &dst);
  static MString WideCharToMString(wchar_t *src, int size);
  static MString *WideCharToMStringAlloc(wchar_t *src, int size);
  static wchar_t *WinXMLUtils::CharToWideChar(const char *str, int size);
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* !__WINXMLUTILS_H__ */
