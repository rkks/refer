/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _SQL_EXECUTOR_H
# define _SQL_EXECUTOR_H_

# include "IDBMS.h"
# include "DefaultNamespace.h"
DNSPACE_OPEN

class MString;
class ObjectInterface;
class MutableDataAnswer;
class MutableObjectInstance;

class SQLExecutor
{
public:
  SQLExecutor(void);
  ~SQLExecutor(void);
  bool Execute(const MString &request, const ObjectInterface &requestOn, const ObjectInterface &answerOn);
  void FillResult(const MString &request, MutableDataAnswer& result);
  const char *GetLastError(void);

private:
  const ObjectInterface          *m_requestOn;
  const ObjectInterface          *m_answerOn;
  void                           *m_rowHandle;
  IDBMS                          *m_sql;
  char                           *m_lastError;
  unsigned char                  *m_cachedata;
  unsigned int                   m_cachelen;

  const ObjectInterface          *GetGoodObjectInterface(unsigned int index, unsigned int *good);
  void                           FillColumn(COLUMN *column, unsigned int index, MutableObjectInstance& objInstance);
};

DNSPACE_CLOSE
#endif /* !_SQL_EXECUTOR_H */
