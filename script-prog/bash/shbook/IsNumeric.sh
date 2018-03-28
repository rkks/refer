
#
# File: IsNumeric.sh
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

IsNumeric() {
     #
     # NAME
     #    IsNumeric - determine if a string is numeric
     #
     # SYNOPSIS
     #    IsNumeric string
     #
     # DESCRIPTION
     #    This function will return true (0) if the string
     #    contains all numeric characters; otherwise, it
     #    will return false (1).
     #    
     if [ $# -ne 1 ]; then
          return 1
     fi

     expr "$1" + 1 >/dev/null 2>&1
     if [ $? -ge 2 ]; then
          return 1
     fi

     return 0
}
