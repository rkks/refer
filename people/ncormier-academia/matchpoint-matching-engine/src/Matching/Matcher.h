/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _MATCHER_H_
# define _MATCHER_H_

# include "DefaultNamespace.h"
DNSPACE_OPEN

class MutableObjectInstance;
class ProfileInterface;
class MatchedObjectInstanceAttribute;
class MatchTask;
class DataAnswer;
class ObjectInstance;

class Matcher
{
public:
  static void Process(MatchTask& tsk);

private:
  Matcher( void );
  ~Matcher(void);
  void Match( MatchTask& tsk, const DataAnswer &dataAnswer );
};

DNSPACE_CLOSE

#endif /* !_MATCHER_H_ */
