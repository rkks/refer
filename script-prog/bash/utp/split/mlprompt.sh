# A simple multiline prompt for bash that looks like this:
#
#    (blank line)
#    hostname:directory
#    historynum $
#
# To remove the blank line before the prompt, delete the first \n:
PS1='\n\h:\W\n\! \$ '

# # A simple multiline prompt for ksh which looks like the bash version.
# # The ${PWD##*/} edits the value of $PWD to just the basename.
# # To remove the blank line before the prompt, join the first two lines:
# hostname=`uname -n`
# PS1='
# ${hostname}:${PWD##*/}
# ! $ '
