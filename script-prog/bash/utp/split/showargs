#!/bin/bash
# showargs: show command-line argument(s) one by one.

# If one arg, echo "argument"; otherwise "arguments":
test $# -ne 1 && s=s
echo "I got $# argument$s:"
# Show arguments one by one; bash echo -E prevents interpretation:
for arg
do echo -E ">>$arg<<"
done
