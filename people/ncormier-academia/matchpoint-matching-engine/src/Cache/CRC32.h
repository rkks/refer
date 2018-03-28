/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#ifndef _CRC32_H_
# define _CRC32_H_

# include "DefaultNamespace.h"

DNSPACE_OPEN

class CRC32
{
 public:
  CRC32(void);
  unsigned int Compute(const void* buffer, unsigned int count);

 private:
  unsigned int m_crc;

  void Compute(unsigned char value);
};

DNSPACE_CLOSE
#endif /* !_CRC32_H_ */
