#!/bin/sh
#
# usage: age_files [directory ...]
# lists size of files by age for a specified directory (default to the
# current directory).
#
# pick which version of ls you use (uncomment one or the other):
#     System V
#LS="ls -ls"
#     Berkeley
LS="ls -lsg"
#
# Print output in form: `Total_size_in_kilobytes	year_or_month'

find ${*:-.} -type f -print | xargs $LS | awk  '
# argument 7 is the month; argument 9 is either hh:mm or yyyy
# test if argument $9 is hh:mm or yyyy format
{
	if ($9 !~ /:/) {
                sz[$9]+=$1;
        } else {
                sz[$7]+=$1;
        }
}
END {
        for (i in sz) printf("%d\t%s\n", sz[i], i);

}' | sort -nr	# Sort the output by numeral, in reverse
