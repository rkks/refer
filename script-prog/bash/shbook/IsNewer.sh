
#
# File: IsNewer.sh
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

IsNewer() {
     #
     # NAME
     #    IsNewer - compare the dates of two files
     #
     # SYNOPSIS
     #    IsNewer file1 file2
     #
     # DESCRIPTION
     #    This function will return true (0) if file1 has
     #    been modified more recently that file2; otherwise,
     #    it will return false (1).
     #    
     if [ $# -ne 2 ]; then
          echo "Usage: IsNewer file1 file2" 1>&2
          exit 1
     fi

     if [ ! -f $1 -o ! -f $2 ]; then
          return 1       # No
     fi

     if [ -n "`find $1 -newer $2 -print`" ]; then
          return 0       # Yes
     else
          return 1       # No
     fi
}
