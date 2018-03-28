/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    TmpMain.cpp
 * Desc    :    Temporary main
 * Author  :    nico
 *****************************************************************************/

#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif
#include "CoreEntity.h"
#include "CommandLine.h"

// TMP
// unix: daemon
// windows: service
int main(int argc, char** argv)
{
  mtp::CommandLineHandler::MainMutableCommandLine().Parse(argc, argv);
  mtp::CoreEntity::LaunchCore();
#ifdef _WIN32
  while (true) ::Sleep(100);
#else
  while (true) ::sleep(1);
#endif

}
