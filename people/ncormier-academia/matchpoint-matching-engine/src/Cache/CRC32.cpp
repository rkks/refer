/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :   
 * Desc    :    
 * Author  :    mat
 *****************************************************************************/

#include "CRC32.h"

DNSPACE

CRC32::CRC32(void)
{
  m_crc = 0xffffffff;
}

unsigned int CRC32::Compute(const void *buffer, unsigned int count)
{
  const unsigned char *ptr = (const unsigned char *) buffer;

  m_crc = 0xffffffff;

  while (count--)
    {
      Compute(*ptr++);
    }

  return m_crc;
}

void CRC32::Compute(unsigned char value)
{
  m_crc ^= ((unsigned int) (value << 24));

  for (int i = 0; i < 8; i++)
    {
      if (m_crc & 0x80000000)
	{
	  m_crc = (m_crc << 1) ^ 0x04C11DB7;
	}
      else
	{
	  m_crc <<= 1;
	}
    }
}
