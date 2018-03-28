#!/bin/sh

# $Id: build.sh,v 1.1 2003/04/05 00:41:50 zull Exp $

LOCAL_ANT_HOME=../../../../tools/ant/
ANT_CLASSPATH=$JAVA_HOME/lib/tools.jar
ANT_CLASSPATH=$LOCAL_ANT_HOME/lib/ant.jar:$LOCAL_ANT_HOME/lib/jaxp.jar:$LOCAL_ANT_HOME/lib/parser.jar:$ANT_CLASSPATH
PATH=$JAVA_HOME/bin:$PATH
$JAVA_HOME/bin/java -classpath "$ANT_CLASSPATH" -Dant.home=$LOCAL_ANT_HOME -DJ2EE.HOME=$J2EE_HOME -DJ2MEWTK.HOME=$J2MEWTK_HOME -DJAVA.HOME=$JAVA_HOME  -DJ2EE_SCRIPT_SUFFIX= org.apache.tools.ant.Main "$@"
