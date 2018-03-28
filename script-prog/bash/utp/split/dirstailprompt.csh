# This makes a multi-line prompt that shows the directory stack; each
# directory name shown is the tail (the lowest-level directory).
# These commands should be put in your .cshrc file.
#
# The prompt will look something like:
# <blank line>
# username@hostname directory_stack
# history_number % 
#
# (You may need to replace ${USER} with ${LOGNAME}.)
#
# The pushd and popd commands are used to push and pop directories
# onto the stack.
#
# This makes a blank line before each prompt; to avoid that, join the
# first and second lines of the setprompt alias.

# Put hostname.domain.name in $hostname and hostname in $HOST:
set hostname=`uname -n`
setenv HOST `expr $hostname : '\([^.]*\).*'`
alias setprompt 'set dirs=(`dirs`); set prompt="\\
${USER}@${HOST} $dirs:gt\\
\! % "'
alias cd  'chdir \!* && setprompt'
alias pushd  'pushd \!* && setprompt'
alias popd  'popd  \!* && setprompt'
setprompt   # Set the initial prompt
