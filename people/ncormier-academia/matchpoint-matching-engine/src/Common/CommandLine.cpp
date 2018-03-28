/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    CommandLine.cpp
 * Desc    :
 * Author  :    nico
 *****************************************************************************/

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
#include <stdlib.h>

#include "CommandLine.h"
#include "MString.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
DNSPACE
/*****************************************************************************/
/* Static variables                                                          */
/*****************************************************************************/
MutableCommandLine* CommandLineHandler::s_mutableCmdLineInstance = NULL;
/*****************************************************************************/
/* Class implementation                                                      */
/*****************************************************************************/
/*
 *  CommandLine implementation
 */
CommandLine::CommandLine(void)
{
  // Objects creation
  m_configFilePath = new MString("examples/conf.xml");
}

CommandLine::~CommandLine(void)
{
  // Objects cleanup
  if (m_configFilePath)   delete m_configFilePath;
}

const MString&  CommandLine::ConfigFilePath(void)
{
  return *m_configFilePath;
}

/*
 *  MutableConfig implementation
 */
MutableCommandLine::MutableCommandLine(void)
{
}

MutableCommandLine::~MutableCommandLine(void)
{
}

bool  MutableCommandLine::Parse(int argc, char** argv)
{
  // Temporary !
  for (int i = 0; i < argc; i++)
  {
    if (MString(argv[i]) == "-c" && argv[i + 1])
      *m_configFilePath = MString(argv[i + 1]);
    else
      if (MString(argv[i]) == "-h" )
	{
	  printf("%s [options]\n", argv[0]);
	  printf("options:\n");
	  printf("\t-c config_file\t: specify an alternate config file\n");
	  printf("\t-h\t\t: this message\n");
	  ::exit(0);
	}
  }

  printf("Using config file `%s`\n", m_configFilePath->CharArray());

  return true;
}

/*
 *  CommandLineHandler implementation
 */
CommandLine&  CommandLineHandler::MainCommandLine(void)
{
  return MainMutableCommandLine();
}

MutableCommandLine& CommandLineHandler::MainMutableCommandLine(void)
{
  if (!s_mutableCmdLineInstance)
    s_mutableCmdLineInstance = new MutableCommandLine();
  return *s_mutableCmdLineInstance;
}
