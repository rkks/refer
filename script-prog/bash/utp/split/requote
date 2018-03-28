#!/bin/sh
# requote - Neaten email quotes prefixed with "> ".
# Reads standard input, writes standard output.
# Any arguments are passed to fmt.

# To be sure we collect all text before writing any (so we can copy and
# paste in the same xterm), use a temp file:
temp=/tmp/REQUOTE$$
umask 077
trap 'rm -f $temp; exit' 0 1 2 15

# Read from stdin, stripping leading '> ' (doesn't attempt multiple >>>):
sed 's/^> //' |
# Read from pipe, reformat (with workaround for old sh argument problem):
    fmt ${1+"$@"} > $temp
# Add '> ' to each reformatted line, write to stdout:
sed 's/^/> /' $temp
