#!/bin/sh
#
# Usage: lookfor <directory> <-/+days> <word> <word> ...
# lookfor - look for all files in the specified directory hierarchy
# modified within a certain time, and pass the resulting names to
# grep to scan for a particular pattern.

# Temporary file to store find results
temp=/tmp/lookfor$$
# Clean up if we're interrupted
trap 'rm -f $temp; exit' 0 1 2 15

# Find all files in the specified directory that were modified within
# the specified time
find $1 -mtime $2 -print > $temp

# shift the directory name and modification time off the command line
shift; shift

# Look for each word (ignoring case) in the result of the find
for word
do grep -i "$word" `cat $temp` /dev/null
done
