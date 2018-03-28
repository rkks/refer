/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    IDBMS.cpp
* Desc    :    
* Author  :    aurelien
*****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <stdlib.h>

#include "IDBMS.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
void	IDBMS::FreeRow(ROW *row)
{
  if (row == NULL)
    return ;

  COLUMNS::const_iterator iter;

  for (iter = row->r_columns.begin(); iter != row->r_columns.end(); iter++)
  {
    if ((*iter).second->c_type == IDBMS_String)
      if ((*iter).second->c_string != NULL)
        free((*iter).second->c_string);
    else
      if ((*iter).second->c_type == IDBMS_Binary)
        free((*iter).second->c_binary);

    delete (*iter).second;
  }

  row->r_columns.clear();
  delete row;
}

COLUMN				*IDBMS::GetColumn(ROW *row,
                                const char *column)
{
  COLUMNS::const_iterator	iter;

  iter = row->r_columns.find(column);
  if (iter == row->r_columns.end())
    return (NULL);

  return ((*iter).second);
}
