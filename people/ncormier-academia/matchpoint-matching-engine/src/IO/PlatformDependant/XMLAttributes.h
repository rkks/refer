/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    XMLAttributes.h
 * Desc    :    XML Node header
 * Author  :    mat
 ******************************************************************************/

#ifndef __XMLATTRIBUTES_H__
# define __XMLATTRIBUTES_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "MString.h"
# include <map>

#ifdef _WIN32
# define strcasecmp _stricmp
#endif /* !_WIN32 */

using namespace std;

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/

typedef struct lstr
{
  bool operator()(MString s1, MString s2) const
  {
    //return (!(s1 == s2));
    return (strcasecmp(s1.CharArray(), s2.CharArray()) > 0);
  }
} strcomparer;


typedef std::map<MString, MString, strcomparer> AttributesMap;

/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class XMLAttributes
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  XMLAttributes();
  virtual ~XMLAttributes();

  bool GetValue(const MString &name, MString &o_val);

private:
  void FreeAttributes();

  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  AttributesMap m_attrs;
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __XMLATTRIBUTES_H__ */
