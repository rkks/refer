rem $Id: build.bat,v 1.3 2003/03/26 01:08:41 zull Exp $

@echo off
set LOCAL_ANT_HOME=../../tools/ant/
set ANT_CLASSPATH=%JAVA_HOME%/lib/tools.jar
set ANT_CLASSPATH=%LOCAL_ANT_HOME%/lib/ant.jar;%LOCAL_ANT_HOME%/lib/jaxp.jar;%LOCAL_ANT_HOME%/lib/parser.jar;%ANT_CLASSPATH%
set PATH=%JAVA_HOME%\bin;%PATH%
%JAVA_HOME%\bin\java -classpath "%ANT_CLASSPATH%" -Dant.home=%LOCAL_ANT_HOME% -DJ2EE.HOME=%J2EE_HOME% -DJ2MEWTK.HOME=%J2MEWTK_HOME% -DJAVA.HOME=%JAVA_HOME%  -DJ2EE_SCRIPT_SUFFIX=.bat org.apache.tools.ant.Main %1 %2 %3 %4 %5 %6 %7 %8 %9
