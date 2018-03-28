/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibDBI.cpp
 * Desc    :    
 * Author  :    aurelien
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <stdio.h>
#include <string.h>

#include "LibDBI.h"
#include "Assert.h"

#include <dbi/dbi.h>
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
LibDBI::LibDBI(void) : IDBMS()
{
}

LibDBI::~LibDBI(void)
{
}

bool	LibDBI::Init(void *arg)
{
  int	r;

  r = dbi_initialize(NULL);

  if (r == -1)
    return (false);

  m_handle = (void *) dbi_conn_new((const char *) arg);

  if (m_handle == NULL)
    {
      FASSERT( false, "no valid DBI driver found\n" );
      return false;  /* XXX exit ? */
    }

  return (true);
}

void	LibDBI::Uninit(void)
{
  dbi_shutdown();
}

const char	*LibDBI::GetError(void)
{
  const char   	*err;

  dbi_conn_error((dbi_conn) m_handle, &err);

  return (err);
}

bool		LibDBI::Open(const char *host, short port,
			     const char *database, const char *username,
			     const char *password)
{
  dbi_conn	db = (dbi_conn) m_handle;

  dbi_conn_set_option(db, "host",  host);
  dbi_conn_set_option_numeric(db, "port", port);
  dbi_conn_set_option(db, "username", username);
  dbi_conn_set_option(db, "password", password);
  dbi_conn_set_option(db, "dbname", database);

  // XXX Si la db n'existe pas il se connect quand meme et ca segfault ...

  if (dbi_conn_connect(db) != 0)
    return (false);

//   if (dbi_conn_select_db(db, database) != 0)
//     {
//       Close();
//       return (false);
//     }

  return (true);
}

void	LibDBI::Close(void)
{
  dbi_conn_close((dbi_conn) m_handle);
}

void	*LibDBI::Execute(const char *query)
{
  return (dbi_conn_query((dbi_conn) m_handle, query));
}

ROW    	*LibDBI::GetNextRow(void *hRow, const char **columns)
{
  if (dbi_result_next_row((dbi_result) hRow) == 0)
    return (NULL);

  return (BuildRow(hRow, columns));
}

ROW	*LibDBI::GetPrevRow(void *hRow, const char **columns)
{
  if (dbi_result_prev_row((dbi_result) hRow) == 0)
    return (NULL);

  return (BuildRow(hRow, columns));
}

void	LibDBI::FreeHRow(void *hRow)
{
  dbi_result_free((dbi_result) hRow);
}

ROW	*LibDBI::BuildRow(void *hRow, const char **columns)
{
  ROW	*row = new ROW;

  COLUMN *column = NULL;
  for (int i = 0; columns[i] != NULL; i++)
    {
      column = new COLUMN;
      column->c_name = columns[i];
      row->r_columns[columns[i]] = column;
      column->c_type = IDBMS_Unknown;
      column->c_size = 0;

      switch (dbi_result_get_field_type(hRow, columns[i]))
	{
	  // TinyInt, BigInt, etc etc
	case (DBI_TYPE_INTEGER):
	  switch (dbi_result_get_field_attribs(hRow, columns[i]) & ~DBI_INTEGER_UNSIGNED)
	    {
	      // char
	    case (DBI_INTEGER_SIZE1):
	      column->c_char = dbi_result_get_char(hRow, columns[i]);
	      column->c_type = IDBMS_Char;
	      break;

	      // short
	    case (DBI_INTEGER_SIZE2):
	      column->c_short = dbi_result_get_short(hRow, columns[i]);
	      column->c_type = IDBMS_Short;
	      break;

	      // int
	    case (DBI_INTEGER_SIZE4):
	      column->c_long = dbi_result_get_long(hRow, columns[i]);
	      column->c_type = IDBMS_Long;
	      break;

	      // long long
	    case (DBI_INTEGER_SIZE8):
	      column->c_longlong = dbi_result_get_longlong(hRow, columns[i]);
	      column->c_type = IDBMS_LongLong;
	      break;
	    }
	  break;

	  // float, double
	case (DBI_TYPE_DECIMAL):
	  switch (dbi_result_get_field_attribs(hRow, columns[i]) & ~DBI_DECIMAL_UNSIGNED)
	    {
	      // float
	    case (DBI_DECIMAL_SIZE4):
	      column->c_float = dbi_result_get_float(hRow, columns[i]);
	      column->c_type = IDBMS_Float;
	      break;

	      // double
	    case (DBI_DECIMAL_SIZE8):
	      column->c_double = dbi_result_get_double(hRow, columns[i]);
	      column->c_type = IDBMS_Double;
	      break;
	    }
	  break;

	  // string
	case (DBI_TYPE_STRING):
	  column->c_string = dbi_result_get_string_copy(hRow, columns[i]);
	  column->c_type = IDBMS_String;
	  column->c_size = dbi_result_get_field_length(hRow, columns[i]) + 1;
	  break;

	  // binary
	case (DBI_TYPE_BINARY):
	  column->c_binary = dbi_result_get_binary_copy(hRow, columns[i]);
	  column->c_type = IDBMS_Binary;
	  column->c_size = dbi_result_get_field_length(hRow, columns[i]) + 1;
	  break;

	  // datetime
	case (DBI_TYPE_DATETIME):
	  column->c_datetime = dbi_result_get_datetime(hRow, columns[i]);
	  column->c_type = IDBMS_Datetime;
	  break;
	}
    }

  return (row);
}
