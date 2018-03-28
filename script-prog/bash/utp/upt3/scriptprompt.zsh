# While running "script", change the zsh shell prompt automatically
# so I remember that I'm logging everything in the terminal.
#
# Put these lines in a zsh setup file like .zshrc.

# Make /usr/bin/script run with SCRIPT environment variable set:
alias script='SCRIPT=yes /usr/bin/script'

#
# Function that sets shell prompt by testing whether SCRIPT is set.
# Call this wherever you set a prompt.
#
# If you don't use the MH email system, delete the \$(folder -list) from
# the second line of the prompt and join the next line of the prompt to it:
#
setprompt( ) {
    case "${TTY##*/}" in
    tty[1-9]) xpi=':tty%l' ;;  # Virtual console
    *) xpi= ;;
    esac

    PS1="
$USER@%m$xpi $(dirs)
%* \$(folder -list)
${SCRIPT+SCRIPT-}%!%# "
}
