/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    ODBC.cpp
* Desc    :    
* Author  :    aurelien
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include "ODBC.h"
#include "Assert.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
ODBC::ODBC(void) : IDBMS()
{
  m_DSN = NULL;
  m_hEnv = NULL;
  m_hConn = NULL;
}

ODBC::~ODBC(void)
{
}

bool	ODBC::Init(void *arg)
{
  if (m_DSN != NULL)
    return (false);

  m_DSN = _strdup((char *) arg);

  if (m_DSN == NULL)
    return (false);

  return (true);
}

void	ODBC::Uninit(void)
{
  free((void *) m_DSN);
}

const char	  *ODBC::GetError(void)
{
  static char buf[255];
  char        code[6];
  SQLINTEGER  native;
  SQLSMALLINT len;
  SQLHANDLE   handle;
  SQLSMALLINT hType;

  if (m_hConn == NULL)
  {
    handle = m_hEnv;
    hType = SQL_HANDLE_ENV;
  }
  else
  {
    handle = m_hConn;
    hType = SQL_HANDLE_DBC;
  }

  SQLRETURN ret;
  try
  {
    ret = SQL_SUCCESS;
    throw 1;
    ret = SQLGetDiagRec(hType, handle, 1, (SQLCHAR *) code, &native, (SQLCHAR *) buf, sizeof(buf), &len);
  }

  catch (...)
  {
    return ("SQLGetDiagRec() crashed, no error message available");
  }

  if (SQL_SUCCEEDED(ret) == 0)
    return ("Unknown error");

  return (buf);
}

bool		    ODBC::Open(const char *host, short port,
                       const char *database, const char *username,
                       const char *password)
{
  SQLHANDLE hEnv;
  SQLHANDLE hConn;

  if ((m_DSN == NULL) || (m_hEnv != NULL))
    return (false);

  if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
    return (false);

  m_hEnv = hEnv;

  if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0) != SQL_SUCCESS)
  {
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    return (false);
  }

  if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hConn) != SQL_SUCCESS)
  {
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    return (false);
  }

  m_hConn = hConn;

  SQLRETURN ret = SQLConnect(hConn, (SQLCHAR *) m_DSN, SQL_NTS, (SQLCHAR *) username, SQL_NTS, (SQLCHAR *) password, SQL_NTS);
  if (SQL_SUCCEEDED(ret) == 0)
  {
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    SQLFreeHandle(SQL_HANDLE_DBC, hConn);
    return (false);
  }

  return (true);
}

void	ODBC::Close(void)
{
  SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
  SQLFreeHandle(SQL_HANDLE_DBC, m_hConn);
  m_hEnv = NULL;
  m_hConn = NULL;
}

void	      *ODBC::Execute(const char *query)
{
  SQLHANDLE hStatement;

  if (SQLAllocHandle(SQL_HANDLE_STMT, m_hConn, &hStatement) != SQL_SUCCESS)
    return (NULL);

  SQLRETURN ret = SQLExecDirect(hStatement, (SQLCHAR *) query, SQL_NTS);
  if (SQL_SUCCEEDED(ret) == 0)
  {
    SQLFreeHandle(SQL_HANDLE_STMT, hStatement);
    return (NULL);
  }

  return (hStatement);
}

ROW    	    *ODBC::GetNextRow(void *hRow, const char **columns)
{
  SQLRETURN ret = SQLFetchScroll((SQLHANDLE) hRow, SQL_FETCH_NEXT, 0);
  if (SQL_SUCCEEDED(ret) == 0)
    return (NULL);

  return (BuildRow(hRow, columns));
}

ROW	        *ODBC::GetPrevRow(void *hRow, const char **columns)
{
  SQLRETURN ret = SQLFetchScroll((SQLHANDLE) hRow, SQL_FETCH_PREV, 0);
  if (SQL_SUCCEEDED(ret) == 0)
    return (NULL);

  return (BuildRow(hRow, columns));
}

void	ODBC::FreeHRow(void *hRow)
{
  SQLFreeHandle(SQL_HANDLE_STMT, (SQLHANDLE) hRow);
}

ROW		*ODBC::BuildRow(void *hRow, const char **columns)
{
  ROW   *row = new ROW;

  COLUMN *column = NULL;
  SQLSMALLINT type;
  SQLRETURN ret;
  char buf[512];

  for (int i = 0; columns[i] != NULL; i++)
  {
    column = new COLUMN;
    ZeroMemory(column, sizeof(*column));
    column->c_name = _strdup(columns[i]);
    row->r_columns[columns[i]] = column;
    ret = SQLColAttributes((SQLHANDLE) hRow, i + 1, SQL_DESC_LENGTH, NULL, 0, NULL, (SQLINTEGER *)&column->c_size);
    if (SQL_SUCCEEDED(ret) == 0)
      return FreeRow(row), NULL;

    type = 0;
    SQLCHAR name[32];

    ret = SQLDescribeCol((SQLHANDLE) hRow, i + 1, name, sizeof(name), NULL, &type, NULL, NULL, NULL);
    if (SQL_SUCCEEDED(ret) == 0)
      return FreeRow(row), NULL;

    SQLINTEGER size = 0;
    switch (type)
    {
    case (SQL_TINYINT):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_LONG, &column->c_char, sizeof(column->c_char), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Char;
      break;

    case (SQL_SMALLINT):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_SHORT, &column->c_short, sizeof(column->c_short), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Short;
      break;

    case (SQL_INTEGER):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_LONG, &column->c_long, sizeof(column->c_long), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Long;
      break;

    case (SQL_BIGINT):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_UBIGINT, &column->c_longlong, sizeof(column->c_longlong), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_LongLong;
      break;

    case (SQL_DOUBLE):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_DOUBLE, &column->c_double, sizeof(column->c_double), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Double;
      break;

    case (SQL_REAL):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_FLOAT, &column->c_float, sizeof(column->c_float), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Float;
      break;

    case (SQL_CHAR):
    case (SQL_VARCHAR):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_C_CHAR, buf, sizeof(buf), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_String;
      column->c_string = _strdup(buf);
      break;

    case (SQL_BINARY):
      // XXX
      column->c_binary = NULL;
      column->c_type = IDBMS_Binary;
      break;

    case (SQL_TYPE_TIMESTAMP):
      if (XSQLGetData((SQLHANDLE) hRow, i + 1, SQL_DATETIME, &column->c_datetime, sizeof(column->c_datetime), &size) != true)
        return FreeRow(row), NULL;
      column->c_type = IDBMS_Datetime;
      break;

    default:
      column->c_type = IDBMS_Unknown;
    }
  }

  return (row);
}

bool        ODBC::XSQLGetData(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLINTEGER BufferLength, SQLINTEGER *StrLen_or_IndPtr)
{
  SQLRETURN ret = SQLGetData(StatementHandle, ColumnNumber, TargetType, TargetValuePtr, BufferLength, StrLen_or_IndPtr);

  if (SQL_SUCCEEDED(ret) == 0)
    return (false);

  return (true);
}
