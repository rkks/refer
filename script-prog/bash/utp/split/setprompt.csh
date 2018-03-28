# Sample C shell alias that sets the prompt.  This can be a single
# set of code that's called by all other aliases that need to reset
# the prompt.  You can use variables, and the output of commands and
# other aliases, to make the prompt you need in a certain case.

# $var1 is a shell variable.
# \!* are the arguments passed to this alias as you invoke it.
# $arg1 is a shell variable with a parameter for command1.
alias setprompt "set prompt = '$var1 \!* `command1 $arg1`% '"
