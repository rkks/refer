/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    Assert.cpp
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include "Assert.h"
#include "Exception.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
Assert::Assert(bool cond, const char* where, const char* why)
{
  if (!cond)
  {
    std::string errMsg = std::string(why) + std::string(" (")
                       + std::string(where) + std::string(")");
    std::cerr << errMsg.c_str() << std::endl;
      if (errno != 0 )
	std::cerr << strerror(errno) << std::endl;
    throw Exception(errMsg.c_str());
  }
}

Assert::~Assert(void)
{
}
