
#
# File: SystemType.sh
#
# Copyright 1995, by Hewlett-Packard Company
#
# The code in this file is from the book "Shell Programming
# Examples" by Bruce Blinn, published by Prentice Hall.
# This file may be copied free of charge for personal,
# non-commercial use provided that this notice appears in
# all copies of the file.  There is no warranty, either
# expressed or implied, supplied with this code.
#

SystemType() {
     #
     # NAME
     #    SystemType - return the type of the system
     #
     # SYNOPSIS
     #    SystemType
     #
     # DESCRIPTION
     #    This function determines the type of the system on
     #    which it is executing and returns one of the
     #    following strings:
     #    
     #         AIX       DECOSF    HP        SCO
     #         SGI       SOLARIS   SUNBSD    ULTRIX
     #
     #    ULTRIX is returned for Digital Equipment's older
     #    BSD systems and DECOSF is returned for their OSF
     #    based systems.
     #
     #    SUNBSD is returned for Sun BSD systems (versions
     #    4.*) and SOLARIS is returned for Sun System V
     #    systems (versions 5.*).
     #
     #    If this function is unable to determine the type
     #    of the system, it will return an empty string.
     #
     _HOSTNAME=`hostname | sed 's/\..*//'`

     case `uname -s` in
          AIX )          echo AIX                 ;;
          HP-UX )        echo HP                  ;;
          IRIX )         echo SGI                 ;;
          OSF1 )         echo DECOSF              ;;
          ULTRIX )       echo ULTRIX              ;;
          SunOS )        case `uname -r` in
                              4*)  echo SUNBSD    ;;
                              5*)  echo SOLARIS   ;;
                         esac
                         ;;
          $_HOSTNAME )   case `uname -m` in
                              IP*)      echo SGI  ;;
                              i386)     echo SCO  ;;
                         esac
                         ;;
     esac
}
