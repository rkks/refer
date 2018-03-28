/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include "Bitstream.h"
#include "IDBMS.h"

DNSPACE

/* format
 *
 * [nb_rows][nb_col][colname1,colname2,...,colnameN][\0][type1][size1][data1][type2][size2][data2][size3][size3][data3]...
 *
 * nb_rows = 4 bytes
 * nb_col = 1 byte
 * type = 1 byte
 * size = 4 bytes
 */

Bitstream::Bitstream(void)
{
  m_data = NULL;
  m_size = 0;
  m_columns = NULL;
  m_rows = 0;
  m_cols = 0;
}

void Bitstream::StartSerialize(const char **columns)
{
  unsigned i;

  if (m_columns != NULL )
    {
      for (unsigned i = 0; m_columns[i] != NULL; i++)
	free(m_columns[i]);
      free(m_columns);
    }

  m_columns = NULL;
  for (i = 0, m_size = 0; columns != NULL; i++ )
    {
      m_columns = (char**)realloc(m_columns, (i + 1) * sizeof(*m_columns));
      if (columns[i] != NULL)
	{
#ifndef _WIN32
	  m_columns[i] = strdup(columns[i]);
#else
	  m_columns[i] = _strdup(columns[i]);
#endif
	  m_size += (unsigned int)strlen(columns[i]) + 1;
	}
      else
	{
	  m_columns[i] = NULL;
	  break;
	}
    }

  /* add header size */
  m_size += 4 + 1;
  if (m_data != NULL)
    free(m_data);
  m_data = (unsigned char*)malloc(m_size);

  /* set number of cols */
  unsigned char *ptr = m_data + sizeof(int);
  *ptr = i;
  ptr++;
  m_cols = i;

  /* copy columns' name */
  for (unsigned j = 0; j < i; j++)
    {
      memcpy(ptr, columns[j], strlen(columns[j]));
      ptr += strlen(columns[j]);
      *ptr = ',';
      ptr++;
    }

  *(ptr - 1) = 0;
  m_rows = 0;
}

Bitstream::~Bitstream(void)
{
  free(m_data);
  for (unsigned i = 0; m_columns && m_columns[i] != NULL; i++)
    free(m_columns[i]);
  free(m_columns);
}

void Bitstream::StartDeserialize(unsigned char *buf, unsigned int size)
{
  if (m_data != NULL)
    free(m_data);
  m_data = (unsigned char*)malloc(size);
  memcpy(m_data, buf, size);
  m_size = size;

  /* get the number of rows */
  memcpy(&m_rows, m_data, sizeof(int));

  /* get the number of cols */
  m_cols = 0;
  memcpy(&m_cols, m_data + sizeof(int), sizeof(char));

  /* get columns name */
#ifndef _WIN32
  unsigned char *ptr = (unsigned char*)strdup((char*)m_data + sizeof(int));
#else
  unsigned char *ptr = (unsigned char*)_strdup((char*)m_data + sizeof(int));
#endif
  unsigned char *savecopy = ptr;
  char *tok;

  if (m_columns != NULL)
    {
      for (unsigned i = 0; m_columns[i] != NULL; i++)
	free(m_columns[i]);
      free(m_columns);
    }
  m_columns = NULL;
  int cols = 0;
  char *save;

#ifndef _WIN32
  while ((tok = strtok_r((char*)ptr, ",", &save)) != NULL)
#else
  while ((tok = strtok_s((char*)ptr, ",", &save)) != NULL)
#endif
    {
      ptr = NULL;

      m_columns = (char**)realloc(m_columns, (cols + 1) * sizeof(*m_columns));
#ifndef _WIN32
      m_columns[cols++] = strdup(tok);
#else
      m_columns[cols++] = _strdup(tok);
#endif
    }
  m_columns = (char**)realloc(m_columns, (cols + 1) * sizeof(*m_columns));
  m_columns[cols] = NULL;
  free(savecopy);
}

int Bitstream::AddRow(struct _ROW *row)
{
  for (unsigned i = 0; m_columns[i] != NULL; i++)
    {
      COLUMN *column = IDBMS::GetColumn(row, m_columns[i]);
      if (column == NULL)
	return -1;

      int size = 0;

      switch (column->c_type)
	{
	case (IDBMS_Char):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(char));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(char);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_char, sizeof(char));
	  m_size += sizeof(char);
	  break;

	case (IDBMS_Short):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(short));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(short);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_short, sizeof(short));
	  m_size += sizeof(short);
	  break;

	case (IDBMS_Long):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(long));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(long);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_long, sizeof(long));
	  m_size += sizeof(long);
	  break;

	case (IDBMS_LongLong):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(long long));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(long long);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_longlong, sizeof(long long));
	  m_size += sizeof(long long);
	  break;

	case (IDBMS_Double):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(double));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(double);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_double, sizeof(double));
	  m_size += sizeof(double);
	  break;

	case (IDBMS_Float):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(float));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(float);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_float, sizeof(float));
	  m_size += sizeof(float);
	  break;

	case (IDBMS_String):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + column->c_size);
	  m_data[m_size++] = column->c_type & 0xff;
	  size = column->c_size;
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, column->c_string, column->c_size);
	  m_size += column->c_size;
	  break;

	case (IDBMS_Binary):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + column->c_size);
	  m_data[m_size++] = column->c_type & 0xff;
	  size = column->c_size;
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, column->c_binary, column->c_size);
	  m_size += column->c_size;
	  break;

	case (IDBMS_Datetime):
	  m_data = (unsigned char*)realloc(m_data, m_size + sizeof(char) + sizeof(int) + sizeof(int));
	  m_data[m_size++] = column->c_type & 0xff;
	  size = sizeof(int);
	  memcpy(m_data + m_size, &size, sizeof(int));
	  m_size += sizeof(int);
	  memcpy(m_data + m_size, &column->c_datetime, sizeof(int));
	  m_size += sizeof(int);
	  break;
	}
    }

  m_rows++;

  return 0;
}

void Bitstream::EndSerialize(void)
{
  memcpy(m_data, &m_rows, sizeof(int));
}

struct _ROW *Bitstream::GetNextRow(unsigned char **save)
{
  if (*save == NULL)
    {
      /* skip the header */
      char *ptr = strchr((char*)m_data + sizeof(int) + sizeof(char), 0);
      if (ptr == NULL)
	return NULL;
      ptr++;
      *save = (unsigned char*)ptr;
    }

  if (*save >= (m_data + m_size))
    return NULL;

  struct _ROW *row = new struct _ROW;

  for (unsigned i = 0; i < m_cols; i++)
    {
      COLUMN *column = new COLUMN;
      column->c_name = m_columns[i];
      row->r_columns[m_columns[i]] = column;
      column->c_type = IDBMS_Unknown;

      unsigned char *ptr = *save;

      memcpy(&column->c_type, ptr, sizeof(char));
      ptr += sizeof(char);
      column->c_size = 0;
      memcpy(&column->c_size, ptr, sizeof(int));
      ptr += sizeof(int);

      switch (column->c_type)
	{
	case (IDBMS_Char):
	  memcpy(&column->c_char, ptr, sizeof(char));
	  break;

	case (IDBMS_Short):
	  memcpy(&column->c_short, ptr, sizeof(short));
	  break;

	case (IDBMS_Long):
	  memcpy(&column->c_long, ptr, sizeof(long));
	  break;

	case (IDBMS_LongLong):
	  memcpy(&column->c_longlong, ptr, sizeof(long long));
	  break;

	case (IDBMS_Double):
	  memcpy(&column->c_double, ptr, sizeof(double));
	  break;

	case (IDBMS_Float):
	  memcpy(&column->c_float, ptr, sizeof(float));
	  break;

	case (IDBMS_String):
	  column->c_string = (char*)malloc(column->c_size + 1);
	  memcpy(column->c_string, ptr, column->c_size);
	  column->c_string[column->c_size] = 0;
	  break;

	case (IDBMS_Binary):
	  column->c_binary = malloc(column->c_size);
	  memcpy(column->c_binary, ptr, column->c_size);
	  break;

	case (IDBMS_Datetime):
	  memcpy(&column->c_datetime, ptr, sizeof(int));
	  break;
	}

      ptr += column->c_size;

      if ((column->c_type != IDBMS_String) && (column->c_type != IDBMS_Binary))
	column->c_size = 0;

      *save = ptr;
    }

  return row;
}

char **Bitstream::GetColumns(void)
{
  return m_columns;
}

unsigned int Bitstream::GetRowsCount(void)
{
  return m_rows;
}

unsigned char *Bitstream::GetBuffer(void)
{
  return m_data;
}

unsigned int Bitstream::GetSize(void)
{
  return m_size;
}
