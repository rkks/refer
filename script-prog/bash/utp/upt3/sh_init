# Automatic Setup When You Enter and Exit A Directory
#
# When you cd into a directory and a file named `.enter.sh' exists,
# the commands in it will be run by your current shell.
# When you cd out of a directory and a file named `.exit.sh' exists,
# the commands in it will be run by your current shell.
#
# Note that if a cd/pushd/popd command fails (from a bad pathname,
# for instance), the .exit file will already have been sourced;
# you may need to recover by hand.  (The "|| return" will end the
# function execution before the .enter file is sourced.)
# You can work around that problem by adding a "test" cd command
# to the start of the cd and pushd functions.  Here's the revised
# cd function for bash:
#	cd() {
#	   (builtin cd "$@") || return
#	   test -O .exit.sh && . ./.exit.sh
#	   builtin cd "$@"
#	   test -O .enter.sh && . ./.enter.sh
#	}
# If the first cd fails, the subshell will have a nonzero exit status, so
# the || operator will execute the return command to end the function.
# (pushd is harder to fix because you need to test for arguments like +2.)

# Bash version:
cd() {
   test -O .exit.sh && . ./.exit.sh
   builtin cd "$@" || return
   test -O .enter.sh && . ./.enter.sh
}
pushd() {
   test -O .exit.sh && . ./.exit.sh
   builtin pushd "$@" || return
   test -O .enter.sh && . ./.enter.sh
}
popd() {
   # If popd is called with an argument, current directory
   # won't change, so don't source the .exit and .enter files:
   test $# -eq 0 && test -O .exit.sh && . ./.exit.sh
   builtin popd "$@" || return
   test $# -eq 0 && test -O .enter.sh && . ./.enter.sh
}

# 
# If you're using the Korn shell, uncomment the ksh version below
# and comment out the bash version above.
#
# # Korn Shell version (needs an alias to override builtin "cd"):
# alias cd=_cd
# _cd() {
#    test -r .exit.sh && . ./.exit.sh
#    \cd "$@" || return
#    test -r .enter.sh && . ./.enter.sh
# }
# 
