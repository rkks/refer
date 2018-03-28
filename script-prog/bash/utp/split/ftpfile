#!/bin/sh
#
# ftpfile - Use a here document to transfer a file using anonymous ftp
# from a shell script.
#
# Usage: ftpfile machine file
#
# set -x   # Print commands and their arguments as they are executed

SOURCE=$1
FILE=$2
GETHOST="uname -n"
BFILE=`basename $FILE`
ftp -n $SOURCE <<EndFTP
ascii
user anonymous $USER@`$GETHOST`
get $FILE /tmp/$BFILE
EndFTP
