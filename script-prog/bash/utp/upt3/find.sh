# Find files only in the current directory with -prune

# print using -print (complete path name only)
Find() { find . \( -type d ! -name . -prune \) -o \( "$@" -print \); }

# print using -s (gives output similar to `ls -ilds')
Findls() { find . \( -type d ! -name . -prune \) -o \( "$@" -ls \); }
