/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    LibDBI.h
 * Desc    :    
 * Author  :    aurelien
 *****************************************************************************/

#ifndef __LIBDBI_H__
# define __LIBDBI_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "DefaultNamespace.h"
# include "IDBMS.h"
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
class LibDBI : public IDBMS
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
       		LibDBI(void);
		~LibDBI(void);

  bool		Init(void *arg);
  void		Uninit(void);
  const char	*GetError(void);

  bool		Open(const char *host, short port,
		     const char *database, const char *username,
		     const char *password);
  void		Close(void);

  void		*Execute(const char *query);
  ROW		*GetPrevRow(void *hRow, const char **columns);
  ROW		*GetNextRow(void *hRow, const char **columns);
  void		FreeHRow(void *hRow);

 private:
  ROW		*BuildRow(void *hRow, const char **columns);
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __LIBDBI_H__ */
