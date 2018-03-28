# cd with abbreviated pathnames -- Just type the initials (first
# letter, or more) of each directory in the pathname, starting at the
# root direcctory.  Put a period after each part.
#   eg: c u.i.h.  might match with /usr/include/hsfs
#
# THIS VERSION PRINTS No match IF NO MATCH, BUT cd ALIAS WON'T WORK...
alias c 'set d=`echo \!^* | sed "s/\([^.]*\)\./\/\1*/g"`/; echo $d; \\
	if ("$d" == "") echo No match.; if ("$d" != "") cd $d'

# THIS VERSION WORKS WITH cd ALIAS, BUT DOUBLE cd AT THE END IS A KLUDGE:
# alias c 'set d=`echo \!^* | sed "s/\([^.]*\)\./\/\1*/g"`/; echo $d; \\
# 	if ("$d" == "") echo No match.; if ("$d" != "") chdir $d; cd .'
# 
