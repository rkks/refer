/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    CommandLine.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __COMMAND_LINE_H__
# define __COMMAND_LINE_H__

/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"
DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
class MString;
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/
class CommandLine
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
                        CommandLine(void);
  virtual              ~CommandLine(void);
public:
  const MString&        ConfigFilePath(void);
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
  MString*  m_configFilePath;
};
class MutableCommandLine : public CommandLine
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
                        MutableCommandLine(void);
  virtual              ~MutableCommandLine(void);
  bool                  Parse(int argc, char** argv);
};
class CommandLineHandler
{
  //friend class EntryPoint; /* Only EntryPoint can get mutable config object */
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
public:
  static CommandLine&        MainCommandLine(void);
//private:
  static MutableCommandLine& MainMutableCommandLine(void);
  /***************************************************************************/
  /* Private static variables                                                */
  /***************************************************************************/
private:
  static MutableCommandLine* s_mutableCmdLineInstance;
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __COMMAND_LINE_H__ */
