
#
# File: DownShift.sh
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

DownShift() {
     #
     # NAME
     #    DownShift - downshift the characters in a string
     #
     # SYNOPSIS
     #    DownShift string
     #
     # DESCRIPTION
     #    This function will downshift the alphabetic
     #    characters in the string.  Nonalphabetic
     #    characters will not be affected.  The downshifted
     #    string will be written to the standard output.
     #    

     echo "$@" | tr '[A-Z]' '[a-z]'
}
