#! /bin/sh
# checksed - run 'sed -f sedscr' on a file and show diff of what it would do

script=sedscr

# Loop through files.  Print header before each file's results.
# Then run sed and show the changes script would make.
# Pipe the output of all commands to a pager (default: less).
for file
do
	echo "********** < = $file    > = sed output **********"
	sed -f $script "$file" | diff "$file" -
done | ${PAGER-less}
