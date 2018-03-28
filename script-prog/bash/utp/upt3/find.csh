# Find files only in the current directory with -prune

# print using -print (complete path name only)
alias find. 'find . \( -type d ! -name . -prune \) -o \( \!* -print \)'

# print using -s (gives output similar to `ls -ilds')
alias find.ls 'find . \( -type d ! -name . -prune \) -o \( \!* -ls \)'
