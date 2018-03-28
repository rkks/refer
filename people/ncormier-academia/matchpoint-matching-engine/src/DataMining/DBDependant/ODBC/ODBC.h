/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    ODBC.h
* Desc    :    
* Author  :    aurelien
*****************************************************************************/

#ifndef __ODBC_H__
# define __ODBC_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "IDBMS.h"
#include <windows.h>
#include <sqltypes.h>

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE_OPEN
/*****************************************************************************/
/* Forward declaration                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class ODBC : public IDBMS
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  ODBC(void);
  ~ODBC(void);

  bool		    Init(void *arg);
  void		    Uninit(void);
  const char	*GetError(void);

  bool		    Open(const char *host, short port,
                   const char *database, const char *username,
                   const char *password);
  void		    Close(void);

  void		    *Execute(const char *query);
  ROW		      *GetPrevRow(void *hRow, const char **columns);
  ROW		      *GetNextRow(void *hRow, const char **columns);
  void		    FreeHRow(void *hRow);

private:
  const char *m_DSN;
  SQLHANDLE  m_hEnv;
  SQLHANDLE  m_hConn;

  ROW		*BuildRow(void *hRow, const char **columns);
  bool  XSQLGetData(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength, SQLINTEGER *Strlen_or_IndPtr);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __ODBC_H__ */
