
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

SpaceAvail() {
    DIRECTORY=$1
    SYSTEM=`SystemType`
    case $SYSTEM in
        ULTRIX) SPACE=`df $DIRECTORY |
                    sed -n '3p' |
                    awk '{print $4}'`
            ;;
        SUNBSD) SPACE=`df $DIRECTORY |
                    sed -n '2p' |
                    awk '{print $4}'`
            ;;
        SOLARIS | DECOSF)
            SPACE=`df -k $DIRECTORY |
                    sed -n '2p' |
                    awk '{print $4}'`
            ;;
        SGI) SPACE=`df -k $DIRECTORY |
                    sed -n '2p' |
                    awk '{print $5}'`
            ;;
        HP) FILESYS=`/etc/devnm $DIRECTORY |
                    awk '{print $1}'`
            BLOCKS=`df $FILESYS 2>/dev/null |
                    sed -e 's/.*: *//' -e 's/ .*//'`
            SPACE=`expr $BLOCKS / 2`
            ;;
        * ) echo "Unexpected system type ($SYSTEM)." 1>&2
            exit 1
            ;;
    esac

    echo $SPACE
}
