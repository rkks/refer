/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include "SQLExecutor.h"
#include "Attributes.h"
#include "Instances.h"
#include "DataAnswer.h"
#include "MString.h"
#include "Interfaces.h"
#include "Config.h"
#ifdef _WIN32
# include "ODBC.h"
# else
# include "LibDBI.h"
#endif
#include "Bitstream.h"
#include "Memcache.h"

DNSPACE

SQLExecutor::SQLExecutor(void)
{
  m_sql = NULL;
  m_requestOn = NULL;
  m_answerOn = NULL;
#ifndef _WIN32
  m_lastError = strdup("Unknown error");
#else
  m_lastError = _strdup("Unknown error");
#endif
  m_cachedata = NULL;
  m_cachelen = 0;
}

SQLExecutor::~SQLExecutor(void)
{
  if (m_sql != NULL)
  {
    m_sql->Close();
    m_sql->Uninit();
    delete m_sql;
  }

  free(m_lastError);
}

bool SQLExecutor::Execute(const MString &request,
                          const ObjectInterface &requestOn, const ObjectInterface &answerOn)
{
  /* memcache get */
  /* if success
   *   memcache_ok = 1;
   *   return true
   * else
   *  memcache_ok = 0;
   *  le reste
   */

  int len;
  void *res = Memcache::MainHandler().Get(request.CharArray(), &len);

  if (res != NULL)
    {
      m_cachedata = (unsigned char*)res;
      m_cachelen = len;
      m_requestOn = &requestOn;
      m_answerOn = &answerOn;
      return true;
    }
  else
    m_cachedata = false;

  IDBMS *sql;

  if (m_sql == NULL)
  {
#ifdef _WIN32
    sql = new ODBC();
#else
    sql = new LibDBI();
#endif

    if (sql->Init((void *) ConfigHandler::MainConfig().BDDType().CharArray()) == false)
    {
      delete sql;
      return (false);
    }

    if (sql->Open(ConfigHandler::MainConfig().BDDHost().CharArray(), 
                  ConfigHandler::MainConfig().BDDPort().ToInt(), 
                  ConfigHandler::MainConfig().BDDDB().CharArray(), 
                  ConfigHandler::MainConfig().BDDUser().CharArray(), 
                  ConfigHandler::MainConfig().BDDPasswd().CharArray()) == false)
      {
	free(m_lastError);
#ifndef _WIN32
	m_lastError = strdup(sql->GetError());
#else
	m_lastError = _strdup(sql->GetError());
#endif
	delete sql;
	return (false);
      }
  }

  printf("Executing >> %s\n", request.CharArray());

  m_rowHandle = sql->Execute(request.CharArray());

  if (m_rowHandle == NULL)
  {
    free(m_lastError);
#ifndef _WIN32
    m_lastError = strdup(sql->GetError());
#else
    m_lastError = _strdup(sql->GetError());
#endif
    sql->Close();
    sql->Uninit();
    return (false);
  }

  m_sql = sql;
  m_requestOn = &requestOn;
  m_answerOn = &answerOn;

  return (true);
}

void SQLExecutor::FillResult(const MString &request, MutableDataAnswer& result)
{
  FASSERT(m_requestOn != NULL, "Execute has not been called or failed");
  FASSERT(m_answerOn != NULL, "Execute has not been called or failed");

  if (m_cachedata != NULL)
    {
      Bitstream bstream;

      bstream.StartDeserialize(m_cachedata, m_cachelen);

      unsigned char *save = NULL;

      struct _ROW *row;
      char **columns = bstream.GetColumns();

      while ((row = bstream.GetNextRow(&save)) != NULL)
	{
	  MutableObjectInstance& objInstance = result.AddNewObject(*m_requestOn);
	  for (unsigned i = 0; columns[i] != NULL; i++)
	    {
	      COLUMN *column = IDBMS::GetColumn(row, columns[i]);
	      if (column == NULL)
		FASSERT(false, "failled to get data from the cache");

	      FillColumn(column, i, objInstance);
	    }
	  IDBMS::FreeRow(row);
	}
      free(m_cachedata);
      m_cachedata = NULL;
      m_cachelen = 0;

      printf(">> Got data from cache\n");

      return;
    }

  char **columns = new char*[(m_requestOn->AttributeCount() + m_answerOn->AttributeCount() + 1) * sizeof(*columns)];

  for (unsigned i = 0, n = m_requestOn->AttributeCount(); i < n; i++)
  {
    const ObjectAttribute& curAttr = m_requestOn->AttributeAtIndex(i);
    FASSERT(IS_INVALID_OBJECT_ATTRIBUTE(curAttr) == false, "Invalid object attribute");
    // Prefix table ?
#ifndef _WIN32
    columns[i] = strdup(curAttr.Attribute().ColumnName().CharArray());
#else
    columns[i] = _strdup(curAttr.Attribute().ColumnName().CharArray());
#endif
  }

  unsigned idx = m_requestOn->AttributeCount();
  for (unsigned i = 0, n = m_answerOn->AttributeCount(); i < n; i++)
  {
    const ObjectAttribute& curAttr = m_answerOn->AttributeAtIndex(i);
    FASSERT(IS_INVALID_OBJECT_ATTRIBUTE(curAttr) == false, "Invalid object attribute");
    // Prefix table ?

    // verif si ya pas de doublons
    for (unsigned j = 0; j < idx; j++)
      {
	if (strcmp(columns[j], curAttr.Attribute().ColumnName().CharArray()) == 0)
	  goto skip;
      }

#ifndef _WIN32
    columns[idx++] = strdup(curAttr.Attribute().ColumnName().CharArray());
#else
    columns[idx++] = _strdup(curAttr.Attribute().ColumnName().CharArray());
#endif
  skip:
    ;
  }

  columns[idx] = NULL;

  ROW *row = NULL;
  COLUMN *column = NULL;
  SQLAttribute *sqlAttr = NULL;

#ifndef _WIN32
  Bitstream bitstream;

  bitstream.StartSerialize((const char **) columns);
#endif

  while ((row = m_sql->GetNextRow(m_rowHandle, (const char **) columns)) != NULL)
  {
#ifndef _WIN32
    if (bitstream.AddRow(row) == -1)
      {
	IDBMS::FreeRow(row);
	FASSERT(false, "failed to add row to the bitstream!");
      }
#endif

    MutableObjectInstance& objInstance = result.AddNewObject(*m_requestOn);
    for (int i = 0; columns[i] != NULL; i++)
    {
      column = IDBMS::GetColumn(row, columns[i]);
      if (column == NULL)
	{
	  IDBMS::FreeRow(row);
	  continue;
	}
      FillColumn(column, i, objInstance);
    }
    IDBMS::FreeRow(row);
  }

  for (unsigned i = 0, n = m_requestOn->AttributeCount(); i < n; i++)
    free(columns[i]);
  delete [] columns;

  m_sql->FreeHRow(m_rowHandle);

#ifndef _WIN32
  bitstream.EndSerialize();

  if (Memcache::MainHandler().Set(request.CharArray(), bitstream.GetBuffer(), bitstream.GetSize()) == -1)
    {
      printf("!! WARNING : Cache add error\n" );
    }
#endif

  printf(">> Got data from SQL\n");
}

const ObjectInterface *SQLExecutor::GetGoodObjectInterface(unsigned int index, unsigned int *good)
{
  FASSERT(index >= 0, "Invalid index");
  if (index < m_requestOn->AttributeCount())
    {
      *good = index;
      return (m_requestOn);
    }

  *good = index - m_requestOn->AttributeCount();

  return (m_answerOn);
}

void SQLExecutor::FillColumn(COLUMN *column, unsigned int index, MutableObjectInstance& objInstance)
{
  unsigned int good;
  const ObjectInterface *obj = GetGoodObjectInterface(index, &good);
  const SQLAttribute &sqlAttr = obj->AttributeAtIndex(good).Attribute();
  switch (column->c_type)
    {
    case (IDBMS_Long):
      {
	AttributeValue<int> longValue;
	longValue.SetValue(column->c_long);
	objInstance.AddNewAttribute(&sqlAttr, longValue);
      }
      break;

    case (IDBMS_String):
      {
	if (column->c_string != NULL)
          {
            AttributeValue<MString> stringValue;
            stringValue.SetValue(column->c_string);
            objInstance.AddNewAttribute(&sqlAttr, stringValue);
          }
      }
      break;

    case (IDBMS_Datetime):
      {
	AttributeValue<int> datetimeValue;
	datetimeValue.SetValue(column->c_datetime);
	objInstance.AddNewAttribute(&sqlAttr, datetimeValue);
      }
      break;

    default:
      printf("WARNING : unhandled type %i\n", column->c_type);
    }
}

const char *SQLExecutor::GetLastError(void)
{
  return (m_lastError);
}
