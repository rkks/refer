/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    IDBMS.h
 * Desc    :    Database management systems interface
 * Author  :    aurelien
 *****************************************************************************/

#ifndef __IDBMS_H__
# define __IDBMS_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include <map>
# include "DefaultNamespace.h"
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
enum	ColumnType
{
  IDBMS_Unknown,
  IDBMS_Char,
  IDBMS_Short,
  IDBMS_Long,
  IDBMS_LongLong,
  IDBMS_Double,
  IDBMS_Float,
  IDBMS_String,
  IDBMS_Binary,
  IDBMS_Datetime
};

typedef struct		_COLUMN
{
  const char		*c_name;
  ColumnType		c_type;
  unsigned long long    c_size;  /* only valid for STRING or BINARY types */
  union
  {
    char		c_char;
    short		c_short;
    int			c_long;
    long long		c_longlong;
    double		c_double;
    float		c_float;
    char		*c_string;
    void		*c_binary;
    unsigned int 	c_datetime;
  };
}			COLUMN;

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

typedef std::map<const char *, COLUMN *, ltstr>	COLUMNS;

typedef struct	_ROW
{
  COLUMNS	r_columns;
}		ROW;

class IDBMS
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
			IDBMS(void)
    {
      m_handle = NULL;
    }
  virtual		~IDBMS(void)
    {
    }

  virtual bool		Init(void *arg) = 0;
  virtual void		Uninit(void) = 0;
  virtual const char	*GetError(void) = 0;

  virtual bool		Open(const char *host, short port,
			     const char *database, const char *username,
			     const char *password) = 0;
  virtual void		Close(void) = 0;

  virtual void		*Execute(const char *query) = 0;
  virtual ROW		*GetPrevRow(void *hRow, const char **columns) = 0;
  virtual ROW		*GetNextRow(void *hRow, const char **columns) = 0;
  static COLUMN		*GetColumn(ROW *row, const char *column);
  static void		FreeRow(ROW *row);
  virtual void		FreeHRow(void *hRow) = 0;

 protected:
  void			*m_handle;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __IDBMS_H__ */
