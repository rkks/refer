/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibXMLAttributes.cpp
 * Desc    :    XML Attributes Unix implementation
 * Author  :    mat
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include "MSXMLAttributes.h"
#include "WinXMLUtils.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
MSXMLAttributes::MSXMLAttributes(ISAXAttributes __RPC_FAR *attrs)
{
  AddAttributes(attrs);
}

void MSXMLAttributes::AddAttributes(ISAXAttributes __RPC_FAR *attrs)
{
  int attrnbr = 0;
  attrs->getLength(&attrnbr);

  int namelen;
  int valuelen;
  unsigned short *name;
  unsigned short *value;
  for (int i = 0; i < attrnbr; i++)
  {
    namelen = valuelen = 0;
    name = value = NULL;
    attrs->getLocalName(i, &name, &namelen);
    if (name != NULL && namelen != 0)
    {
      attrs->getValueFromName((wchar_t *) "", 0, name, namelen, &value, &valuelen);
      if (value != NULL && valuelen != 0)
      {
        AddAttribute(name, namelen, value, valuelen);
      }
    }
  }
}

void MSXMLAttributes::AddAttribute(wchar_t *name, int namesize, wchar_t *val, int valsize)
{
  m_attrs[WinXMLUtils::WideCharToMString(val, valsize)] = WinXMLUtils::WideCharToMString(val, valsize);
}

MSXMLAttributes::~MSXMLAttributes()
{
}
