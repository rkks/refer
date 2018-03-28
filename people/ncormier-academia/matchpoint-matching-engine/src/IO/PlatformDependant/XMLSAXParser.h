/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLSAXParser.h
 * Desc    :    XML SAX Parser base header
 * Author  :    mat
 ******************************************************************************/

#ifndef __XMLSAXPARSER_H__
# define __XMLSAXPARSER_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class ISAXHandler;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class XMLSAXParser
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
  XMLSAXParser(const char *path, ISAXHandler &handl);
  XMLSAXParser(const char *buffer, int size, ISAXHandler &handl);
public:
  virtual ~XMLSAXParser();
  static XMLSAXParser *Factory(const char *path, ISAXHandler &handl);
  static XMLSAXParser *Factory(const char *buffer, int size, ISAXHandler &handl);
private:
  /***************************************************************************/
  /* Members                                                                 */
  /***************************************************************************/
protected:
  const char *m_path;
  const char *m_buffer;
  int m_bufsize;
  ISAXHandler &m_handl;
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __XMLSAXPARSER_H__ */
