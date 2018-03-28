
#
# File: StrCmp.sh
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

StrCmp() {
     #
     # NAME
     #    StrCmp - compare two strings
     #
     # SYNOPSIS
     #    StrCmp string1 string2
     #
     # DESCRIPTION
     #    This function returns -1, 0, or 1 to indicate
     #    whether string1 is lexicographically less than,
     #    equal to, or greater than string2.  The return
     #    value is written to the standard output, not the
     #    exit status.
     #
     if [ $# -ne 2 ]; then
          echo "Usage: StrCmp string1 string2" 1>&2
          exit 1
     fi
     
     if [ "$1" = "$2" ]; then
          echo "0"
     else
          _TMP=`{ echo "$1"; echo "$2"; }|sort|sed -n '1p'`

          if [ "$_TMP" = "$1" ]; then
               echo "-1"
          else
               echo "1"
          fi
     fi
}
