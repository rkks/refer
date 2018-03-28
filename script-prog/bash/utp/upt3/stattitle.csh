set e=`echo x | tr x '\033'`           # Make an ESCape character
set g=`echo x | tr x '\07'`            # And a Ctrl-g
set host=`uname -n | sed 's/\..*//'`   # Get local part of hostname
# Escape sequences are:
# ${e}7 = save cursor position, ${e}[25;1f = go to start of status
# line (line 25), ${e}[0K = erase line, ${e}8 = restore cursor
alias setstatline 'echo -n "${e}7${e}[25;1f${e}[0K    ${host}:${cwd}${e}8"'
alias settitle 'echo -n "${e}]2;${host}:${cwd}${g}"'
switch ($TERM)
case vt10?:
  # Put $host and $cwd in VT102 status line:
  alias cd 'chdir \!* && setstatline'
  alias pushd '""pushd \!* && setstatline'
  alias popd '""popd \!* && setstatline'
  # If using tcsh, comment out the previous three lines and uncomment this:
  #alias cwdcmd setstatline
  breaksw
case xterm*:
  # Put $host and $cwd in xterm titlebar:
  alias cd 'chdir \!* && settitle'
  alias pushd '""pushd \!* && settitle'
  alias popd '""popd \!* && settitle'
  # If using tcsh, comment out the previous three lines and uncomment this:
  #alias cwdcmd settitle
  breaksw
endsw
