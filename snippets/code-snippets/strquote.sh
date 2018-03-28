#!/bin/sh

# Set the LINE variable to this test data
read LINE <<EOF
"something in quotes"  something_unquoted 'what?  why?'
EOF

echo "Bad splitting:"
echo ================
# What if we want to split this literal string like a shell would?
# Letting quotes define where a parameter stops and starts.
# The shell won't evaluate it when quotes are INSIDE the string,
# and will always split on spaces.
printf "%s\n" $LINE

echo "Good splitting:"
echo "==============="

# xargs DOES handle quote characters, however.
echo "$LINE" | xargs printf "%s\n" > /tmp/$$
while read LINE
do
	echo "$LINE"
done < /tmp/$$
rm -f /tmp/$$
