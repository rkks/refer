/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MSXMLAttributes.h
 * Desc    :    XML Attribute Unix header
 * Author  :    mat
 ******************************************************************************/

#ifndef __MSXMLATTRIBUTES_H__
# define __MSXMLATTRIBUTES_H__

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
class MSXMLAttributes : public XMLAttributes
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  MSXMLAttributes();
  ~MSXMLAttributes();
  MSXMLAttributes(ISAXAttributes __RPC_FAR *attrs);
  void AddAttributes(ISAXAttributes __RPC_FAR *attrs);
  void AddAttribute(unsigned short *name, int namesize, unsigned short *val, int valsize);
private:
  /***************************************************************************/
  /* Members                                                                 */
  /***************************************************************************/
protected:
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE
#endif /* __MSXMLATTRIBUTES_H__ */
