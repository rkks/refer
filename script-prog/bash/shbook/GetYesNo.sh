
#
# File: GetYesNo.sh
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

GetYesNo() {
     #
     # NAME
     #    GetYesNo - ask a yes or no question
     #
     # SYNOPSIS
     #    GetYesNo "message"
     #
     # DESCRIPTION
     #    This function will prompt the user with the
     #    message and wait for the user to answer "yes" or
     #    "no".  This function will return true (0) if the
     #    user answers yes; otherwise, it will return
     #    false (1).
     #
     #    This function will accept y, yes, n, or no, and it
     #    is reasonably tolerant of upper and lower case
     #    letters; any other answer will cause the question
     #    to be repeated.
     #
     _ANSWER=            # Answer read from user

     if [ $# -eq 0 ]; then
          echo "Usage: GetYesNo message" 1>&2
          exit 1
     fi

     while :
     do
          if [ "`echo -n`" = "-n" ]; then
               echo "$@\c"
          else
               echo -n "$@"
          fi
          read _ANSWER
          case "$_ANSWER" in
               [yY] | yes | YES | Yes ) return 0       ;;
               [nN] | no  | NO  | No  ) return 1       ;;
               * ) echo "Please enter y or n."         ;;
          esac
     done
}
