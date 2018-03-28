# This csh alias named C shows how to include an if-else test in an alias.
# The alias definition is inside double quotes with two backslashes at
# the end of each line.  An outer eval command, which is surrounded by
# single quotes, does the work when the alias is invoked.  It's ugly,
# but it works (on the C shells where we've tried it, at least...).

# COMPILE AND chmod C PROGRAMS; DON'T USE .c ON END OF FILENAME.
alias C 'eval "if (\!* =~ *.c) then \\
   echo "C quitting: no .c on end of \!* please." \\
else \\
   if (-e \!*) mv \!* \!*.old \\
   echo \!*.c SENT TO cc \\
   cc -s \!*.c -o \!* \\
   if (-e \!*) chmod 311 \!* \\
endif"'
