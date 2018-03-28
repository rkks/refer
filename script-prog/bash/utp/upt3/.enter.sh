# Sample .enter.sh file for use with the sh-type 'cd' function

# Save previous umask; reset in .exit.sh:
prevumask=`umask`

# Let everyone in the group edit my files here:
umask 002
echo ".enter.sh: setting umask to 002"

# Prompt (with blank line before) to keep me awake:
PS1="
$PWD - PROJECT DEVELOPMENT DIRECTORY.  EDIT CAREFULLY...
$ "
