# Put ESCape character in $e.  Use to start blinking mode (${e}[5m)
# and go back to normal mode (${e}[0m) on VT100-series terminals:
e="`echo x | tr x '\033'`"
PS1="${e}[5mroot${e}[0m@`uname -n`# "

# bash v2 users: comment out the previous two lines, uncomment this one:
#PS1='\[\e[5m\]root\[\e[0m\]@\H\$ '
