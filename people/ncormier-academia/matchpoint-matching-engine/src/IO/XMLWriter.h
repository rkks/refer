/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _XMLWRITER_H
# define _XMLWRITE_H_

# include "DefaultNamespace.h"
DNSPACE_OPEN

class MString;

class XMLWriter
{
 public:
  XMLWriter(MString& output);
  ~XMLWriter(void);

  void OpenNode(const MString &node);
  void OpenNodeWithCDATA(const MString &node);
  void OpenNodeWithAttributes(const MString &node);
  void CloseNode(const MString &node);
  void CloseNodeWithCDATA(const MString &node);
  void AddAttribute(const MString &key, const MString &value);
  void CloseAttributes(void);
  void AddCData(const MString &data);
  void AddSimpleNode(const MString &node);

 private:
  int m_level;
  MString*  m_output;

  void internalWrite(const MString &str);
  void indent(void);
};
DNSPACE_CLOSE
#endif /* !_XMLWRITER_H_ */
