# Sample Bourne-type shell function that sets the prompt.  This can be a
# single set of code that's called by all other aliases that need to reset
# the prompt.  You can use variables, and the output of commands and
# other aliases, to make the prompt you need in a certain case.

setprompt()
{
    # $1 is a parameter passed to this function at invocation
    # $arg1 is a shell variable that's set somewhere
    # Because the prompt string is inside double quotes, this
    # will be a static prompt (command1 will be run just once,
    # and its output will be stored as part of the prompt string).
    PS1="$1 `command1 $arg1`\$ "
}
