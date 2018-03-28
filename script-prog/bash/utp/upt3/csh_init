# Automatic Setup When You Enter and Exit A Directory
#
# When you cd into a directory and a file named `.enter.csh' exists,
# the commands in it will be run by your current C shell.
# When you leave a directory and a file named `.exit.csh' exists,
# the commands in it will be run by your current C shell.
#
# Note that if a cd/pushd/popd command fails (from a bad pathname,
# for instance), the .exit and .enter files will still be sourced;
# you may need to recover by hand.  You could also work around that
# problem by re-writing the aliases to read sourceable script files
# (see Unix Power Tools 3rd edition article 29.7).  Here's the cd alias:
#	alias cd 'set cd_args=(\!*); source ~/lib/csh/cd'
# The ~/lib/csh/cd script file could look like this:
#	if { (chdir $cd_args) } then
#	    if (-o .exit.csh) source .exit.csh
#	    chdir $cd_args
#	    if (-o .enter.csh) source .enter.csh
#	endif
# The first line of the script runs a trial chdir command in a subshell.
# If the subshell returns a nonzero status, then the chdir succeeded
# (but didn't actually change the current shell's current directory
# because it's a subshell... see Unix Power Tools article 24.4.)
#
# Note: pushd is harder to handle because it can have arguments like "+2".
#
# We use the csh command chdir instead of cd to avoid alias loops.
# Unix Power Tools article 29.8 explains the similar ""pushd and ""popd.

# Uncomment the following three aliases for original csh.
#
# alias cd 'if (-o .exit.csh) source .exit.csh; chdir \!*; if (-o .enter.csh) source .enter.csh'
# alias pushd 'if (-o .exit.csh) source .exit.csh; ""pushd \!*; if (-o .enter.csh) source .enter.csh'
# # If popd has an argument (like popd +2), current directory doesn't change,
# # so we don't source the .exit.csh file:
# alias popd 'if ("\!*" == "" && -o .exit.csh) source .exit.csh; ""popd \!*; if ("\!*" == "" && -o .enter.csh) source .enter.csh'

# The aliases below are for the tcsh shell only.  If these don't work
# for you, comment them out and use the original csh aliases above.
#
# The special tcsh alias cwdcmd is run after every change of working directory
alias cwdcmd 'if (-o .enter.csh) source .enter.csh'
# If popd has an argument (like popd +2), current directory doesn't change,
# so we don't source the .exit.csh file:
alias popd 'if ("\!*" == "" && -o .exit.csh) source .exit.csh; ""popd \!*'
alias cd 'if (-o .exit.csh) source .exit.csh; chdir \!*'
alias pushd 'if (-o .exit.csh) source .exit.csh; ""pushd \!*'
