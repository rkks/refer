/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _BITSTREAM_H_
# define _BITSTREAM_H_

# include "DefaultNamespace.h"
DNSPACE_OPEN

struct _ROW;

class Bitstream
{
 public:
  Bitstream(void);
  ~Bitstream(void);
  void StartSerialize(const char **columns);
  void StartDeserialize(unsigned char *buf, unsigned int size);

  int AddRow(struct _ROW *row);
  void EndSerialize(void);

  struct _ROW *GetNextRow(unsigned char **save);

  char **GetColumns(void);
  unsigned int GetRowsCount(void);
  unsigned char *GetBuffer(void);
  unsigned int GetSize(void);

 private:
  unsigned char *m_data;
  unsigned int  m_size;
  char          **m_columns;
  unsigned int  m_rows;
  unsigned int  m_cols;
};

DNSPACE_CLOSE
#endif /* !_BITSTREAM_H_ */
