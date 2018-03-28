
#
# File: CheckHostname.sh
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

CheckHostname() {
     #
     # NAME
     #    CheckHostname - determine if a host name is valid
     #
     # SYNOPSIS
     #    CheckHostname [hostname]
     #
     # DESCRIPTION
     #    This function will return true (0) if the host
     #    name is valid; otherwise, it will return
     #    false (1).  If the host name is omitted, the
     #    current host is checked.
     #
     _PING=                   # Customized ping command
     _HOST=${1:-`hostname`}   # Name of the host to check

     case `uname -s` in
          OSF1 )    _PING="ping -c1 $_HOST"  ;; # DEC OSF
          HP-UX )   _PING="ping $_HOST 64 1" ;;
          IRIX )    _PING="ping -c1 $_HOST"  ;; # SGI
          SunOS )   _PING="ping $_HOST"      ;; # BSD and
                                                # Solaris
          * )       return 1                 ;;
     esac

     if [ `$_PING 2>&1 | grep -ci "Unknown host"` -eq 0 ]
     then
          return 0
     else
          return 1
     fi
}
