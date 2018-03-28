/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    MString.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __MSTRING_H__
# define __MSTRING_H__

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
#ifndef __MSTRING_INTERNAL_TYPE
# define __MSTRING_INTERNAL_TYPE void
#endif /* __MSTRING_INTERNAL_TYPE */
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class MString
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                    MString(void);
                    MString(const MString& str);
                    MString(const char* str);
                    MString(const char* str, int size);
virtual            ~MString(void);
  char              operator[](unsigned i) const;
  bool              operator==(const MString& s) const;
  bool              operator!=(const MString& s) const;
  bool              operator>(const MString& s) const;
  bool              operator<(const MString& s) const;
  MString&          operator+=(const MString& s);
  MString&          operator=(const MString& s);
  MString           SubStr(unsigned pos = 0) const;
  MString           SubStr(unsigned pos, unsigned npos) const;
  const char*       CharArray(void) const;
  int               ToInt(void) const;
  unsigned int      Size(void) const;
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  __MSTRING_INTERNAL_TYPE* m_str; /* Can't do a std::string forward declaration ... */
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __STRING_H__ */
