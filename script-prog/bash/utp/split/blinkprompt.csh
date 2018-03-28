# Put ESCape character in $e.  Use to start blinking mode (${e}[5m)
# and go back to normal mode (${e}[0m) on VT100-series terminals:
set e="`echo x | tr x '\033'`"
set prompt="${e}[5mroot${e}[0m@`uname -n`# "
