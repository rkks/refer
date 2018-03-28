#! /bin/sh
# Sort a list of names by last name.
# Doesn't work for two-word last names like "de Beers"

# Print last field (last name), a TAB, then whole name:
awk '{print $NF "\t" $0}' $* |
# sort (by last name: the temporary first field)
sort |
# strip off first field and print the names:
cut -f2-
