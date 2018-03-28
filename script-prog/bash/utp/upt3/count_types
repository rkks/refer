#!/bin/sh
#
# usage: count_types [directory ...]
# counts how many files there are of each type in a specified
# directory (defaults to the current directory).
#
# Uses an associative array to keep track of the # of files per type
# Print out in the format `#_of_files	file_type' for each type of file
#
# Original by Bruce Barnett
# Updated version by yu@math.duke.edu (Yunliang Yu)

find ${*-.} -type f -print | xargs file |
awk '{
	$1=NULL;
	t[$0]++;
}
END {
	for (i in t) printf("%d\t%s\n", t[i], i);
}' | sort -nr   # Sort the result numerically, in reverse
