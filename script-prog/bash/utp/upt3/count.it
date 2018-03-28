#!/bin/sh

# Show the difference in word count between two files.

count_1=`wc -w < $1`    # number of words in file 1
count_2=`wc -w < $2`    # number of words in file 2

diff_12=`expr $count_1 - $count_2`      # difference in word count

# if $diff_12 is negative, reverse order and don't show the minus sign:
case "$diff_12" in
-*) echo "$2 has `expr $diff_12 : '-\(.*\)'` more words than $1" ;;
*)  echo "$1 has $diff_12 more words than $2" ;;
esac
