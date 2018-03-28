# This makes a multi-line prompt that shows the directory stack.  These
# commands should be put in your .bash_profile and/or .bashrc file.
#
# The prompt will look something like:
# <blank line>
# username@hostname directory_stack
# history_number % 
#
# Notice that the value is stored within single quotes.  This makes a
# dynamic prompt: the $(dirs) command substitution will be done as
# every prompt string is output.
#
# This makes a blank line before each prompt; to avoid that,
# remove the first \n.

PS1='\n\u@\h $(dirs)\n\! \$ '
