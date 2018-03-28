/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _SQL_GENERATOR_H_
# define _SQL_GENERATOR_H_

# include "DefaultNamespace.h"
DNSPACE_OPEN

class MString;
class MatchTask;
class UserRequest;
class MutableObjectInstance;
class ObjectInterface;
class ProfileInterface;
class MatchRules;

class SQLGenerator
{
public:
  static void FillAnswer(MatchTask& tsk);
private:
  SQLGenerator(void);
  ~SQLGenerator(void);
  bool GenerateAndExecute(MatchTask& tsk);

 private:
  void  GenerateSelect(const ObjectInterface &obj, const ObjectInterface &answerOn, const MatchRules &matchRules, MString& sqlRequest);
  const char *EscapeString(const char *p_str);
};

DNSPACE_CLOSE

#endif /* !_SQL_GENERATOR */
