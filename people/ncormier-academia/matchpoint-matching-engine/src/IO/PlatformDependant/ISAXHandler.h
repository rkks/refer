/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    ISAXHandler.h
 * Desc    :    SAX Interface Handler header
 * Author  :    mat
 ******************************************************************************/

#ifndef __ISAXHANDLER_H__
# define __ISAXHANDLER_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "XMLSAXParser.h"
# include "XMLAttributes.h"

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class ISAXHandler
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  virtual void startDocument(XMLSAXParser *parser) = 0;
  virtual void endDocument(XMLSAXParser *parser) = 0;
  virtual void startElement(XMLSAXParser *parser, const char *name, const XMLAttributes &attrs) = 0;
  virtual void endElement(XMLSAXParser *parser, const char *name) = 0;
  virtual void characters(XMLSAXParser *parser, const char *chars, int len) = 0;

private:
  virtual ~ISAXHandler()
  {
  }

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

#endif /* __ISAXHANDLER_H__ */
