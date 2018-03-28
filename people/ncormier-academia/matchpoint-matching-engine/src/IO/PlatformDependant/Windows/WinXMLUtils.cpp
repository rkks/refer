
/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLNode.cpp
 * Desc    :    XML Node implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "WinXMLUtils.h"
#include <windows.h>
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/

char *WinXMLUtils::strndup(const char *src, int size)
{
  char *dst = (char *) malloc((size + 1) * sizeof(*src));
  if (dst == NULL)
  {
    throw;
  }
  strncpy_s(dst, size, src, size);
  dst[size] = NULL;
  return (dst);
}

char *WinXMLUtils::strndup(wchar_t *src, int size)
{
  char *dst = (char *) malloc((size + 1) * sizeof(*src));
  if (dst == NULL)
  {
    throw;
  }
  WideCharToMultiByte(CP_ACP, 0, src, size, dst, size, NULL, NULL);
  dst[size] = NULL;
  return (dst);
}

void WinXMLUtils::WideCharToMString(wchar_t *src, int size, MString &val)
{
  char *buf = WinXMLUtils::strndup(src, size);
  val = MString(buf);
  free(buf);
}

MString WinXMLUtils::WideCharToMString(wchar_t *src, int size)
{
  char *buf = WinXMLUtils::strndup(src, size);
  MString val(buf);
  free(buf);
  return (val);
}

MString *WinXMLUtils::WideCharToMStringAlloc(wchar_t *src, int size)
{
  char *buf = WinXMLUtils::strndup(src, size);
  MString *val = new MString(buf);
  free(buf);
  return (val);
}

wchar_t *WinXMLUtils::CharToWideChar(const char *str, int size)
{
  int ResultLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, reinterpret_cast <const char *>(str), size, NULL, 0);
  wchar_t *dst = SysAllocStringLen(NULL, ResultLength);
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, reinterpret_cast <const char *>(str), size, dst, ResultLength);
  return (dst);
}
