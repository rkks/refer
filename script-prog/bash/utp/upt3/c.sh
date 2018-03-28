function c( )
{
   dir="$1"

   # Delete dots.  Surround every letter with "/" and "*".
   # Add a final "/." to be sure this only matches a directory:
   dirpat="`echo $dir | sed 's/\([^.]*\)\./\/\1*/g'`/."

   # In case $dirpat is empty, set dummy "x" then shift it away:
   set x $dirpat; shift

   # Do the cd if we got one match, else print error:
   if [ "$1" = "$dirpat" ]; then
      # pattern didn't match (shell didn't expand it)
      echo "c: no match for $dirpat" 1>&2
   elif [ $# = 1 ]; then
      echo "$1"
      cd "$1"
   else
      echo "c: too many matches for $dir:" 1>&2
      ls -d "$@"
   fi

   unset dir dirpat
}
