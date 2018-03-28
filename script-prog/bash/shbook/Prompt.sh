
#
# File: Prompt.sh
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

Prompt() {
     #
     # NAME
     #    Prompt - print a message without a newline
     #
     # SYNOPSIS
     #    Prompt [message]
     #
     # DESCRIPTION
     #    This function prints the message to the standard
     #    output without a newline at the end of the line.
     #
     #    If the message is not passed, "> " will be
     #    printed.
     #
     if [ "`echo -n`" = "-n" ]; then
          echo "${@-> }\c"
     else
          echo -n "${@-> }"
     fi
}
