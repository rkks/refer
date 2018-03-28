
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

SpaceUsed() {
    DIRECTORY=$1
    SYSTEM=`SystemType`
    case $SYSTEM in
        SUNBSD | ULTRIX )
            SPACE=`du -s $DIRECTORY
            ;;
        HP | SGI | SOLARIS | DECOSF )
            SPACE=`du -s $DIRECTORY
            SPACE=`expr $SPACE / 2`
            ;;
        * ) echo "Unexpected system type ($SYSTEM)." 1>&2
            exit 1
            ;;
    esac

    echo $SPACE
}
