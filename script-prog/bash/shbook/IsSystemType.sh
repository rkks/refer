
#
# File: IsSystemType.sh
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

IsSystemType() {
     #
     # NAME
     #    IsSystemType - compare string with current system
     #
     # SYNOPSIS
     #    IsSystemType string
     #
     # DESCRIPTION
     #    This function will return true (0) if the string
     #    matches one of the values returned by the uname
     #    command; otherwise, it will return false (1).
     #
     if [ $# -ne 1 ]; then
          echo "Usage: IsSystemType string" 1>&2
          exit 1
     fi

     if [ "$1" = "`uname -s`" ]; then
          return 0
     elif [ "$1" = "`uname -m`" ]; then
          return 0
     else
          case `uname -r` in
               "$1"* ) return 0 ;;
          esac
     fi
     return 1
}
