
#
# File: Clear.sh
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

Clear() {
     #
     # NAME
     #    Clear - clear the terminal screen
     #
     # SYNOPSIS
     #    Clear
     #
     # DESCRIPTION
     #    This function will clear the terminal screen using
     #    either the clear command or the tput command.  If
     #    neither of these commands are available, 40 blank
     #    lines will be printed to clear the screen.
     #
     { clear;       } 2>/dev/null  ||
     { tput clear;  } 2>/dev/null  ||
     for i in 1 2 3 4 5 6 7 8 9 10 \
              1 2 3 4 5 6 7 8 9 20 \
              1 2 3 4 5 6 7 8 9 30 \
              1 2 3 4 5 6 7 8 9 40
     do
          echo
     done
}
