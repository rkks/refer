/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _ANSWER_SERIALIZER_H_
# define _ANSWER_SERIALIZER_H_

# include "DefaultNamespace.h"
DNSPACE_OPEN

class MString;
class XMLWriter;
class MutableMatchedObjectInstance;
class MatchAnswer;
class ObjectInterface;

class AnswerSerializer
{
 public:
  AnswerSerializer(MString& ouput);
  virtual ~AnswerSerializer(void);
  bool Serialize(const MatchAnswer& matchAnswer, const ObjectInterface &answerOn);

 private:
  XMLWriter *m_xml;
  
};

DNSPACE_CLOSE
#endif /* !_ANSWER_SERIALIZER_H_ */
