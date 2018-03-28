# Sample .enter.csh file for use with the C shell 'cd' alias

# Save previous umask; reset in .exit:
set prevumask=`umask`

# Let everyone in the group edit my files here:
umask 002
echo ".enter.csh: setting umask to 002"

# Prompt (with blank line before) to keep me awake:
set prompt="\
$cwd - PROJECT DEVELOPMENT DIRECTORY.  EDIT CAREFULLY...\
% "
