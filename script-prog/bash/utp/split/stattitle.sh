#
# Store an ESCape character in ${e} and CTRL-g (alert/bell) in ${g}:
#
e=`echo -e '\033'`           # bash
#e=`echo x | tr x \\\\033`   # Uncomment if your echo can't handle \nnn  
g=`echo -e '\007'`           # bash
#g=`echo x | tr x \\\\007`   # Uncomment if your echo can't handle \nnn

#
# Store local part of hostname in ${host}:
#
host=`uname -n`; host=${host%.*}
#host=`expr \`uname -n\` : '\([^.]*\).*'`     # If shell doesn't support ${x%y}

#
# In functions that update terminal info, escape sequences are:
# ${e}7 = save cursor position, ${e}[25;1f = go to start of status
# line (line 25), ${e}[0K = erase line, ${e}8 = restore cursor
#
# The case statements below call the correct function for each type
# of terminal.  Extend them to handle all of the terminal types you use.
#
# We've included functions for bash and ksh (both ksh88 and 93).
# You may need to do some hacking for other Bourne-type shells.
#

#
# For bash:
#
setstatline() { echo -n "${e}7${e}[25;1f${e}[0K    ${host}:`pwd`${e}8"; }     
settitle() { echo -n "${e}]2;${host}:`pwd`${g}"; } 

case "$TERM" in
vt10?)
  # Put $host and $cwd in VT102 status line:
  cd() { builtin cd "$@" && setstatline; }
  pushd() { builtin pushd "$@" && setstatline; }
  popd() { builtin popd "$@" && setstatline; }
  ;;
xterm*)
  # Put $host and $cwd in xterm titlebar:
  cd() { builtin cd "$@" && settitle; }
  pushd() { builtin pushd "$@" && settitle; }
  popd() { builtin popd "$@" && settitle; }
  ;;
esac

#
# Uncomment these and comment out the functions above to use
# for ksh:
#
#setstatline() { print -n "${e}7${e}[25;1f${e}[0K    ${host}:`pwd`${e}8"; }     
#settitle() { print -n "${e}]2;${host}:`pwd`${g}"; } 
#alias cd=_cd   
#
#case "$TERM" in
#vt10?)
#  # Put $host and $cwd in VT102 status line:
#  _cd() { \cd "$@" && setstatline; }
#  ;;
#xterm*)
#  # Put $host and $cwd in xterm titlebar:
#  _cd() { \cd "$@" && settitle; }
#  ;;
#esac
