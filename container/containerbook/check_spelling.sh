#!/bin/bash

FILES=$(find . -name *.rst)
ASPELL_CMD="aspell --personal=./.aspell.en.pws"
EXIT_CODE=0

function check {
    MODE=$1
    for file in $FILES; do
        WORDS=$($ASPELL_CMD list < $file)
        if [ ! -z "$WORDS" ]; then
            if [ "$MODE" == "auto" ]; then
                echo "Please check $file for the following words:"
                echo $WORDS
                EXIT_CODE=1
            else
                $ASPELL_CMD check $file
            fi
        fi
    done

    exit $EXIT_CODE
}

function usage {
    echo "USAGE: $0 [OPTIONS..]"
    echo "  -a Do an automatic check. Exit code > 0 means there are spelling"
    echo "     mistakes"
    echo "  -h Show this help"
    exit 1
}

options=':ah'
while getopts $options option
do
    case $option in
        a  )    mode="auto";;
        h  )    usage;;
    esac
done

shift $(($OPTIND - 1))

check $mode
