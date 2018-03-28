# Make a simple multiline prompt for C shells

set hostname=`uname -n`
alias setprompt 'set prompt="\\
${hostname}:${cwd}\\
\! % "'
alias cd 'chdir \!* && setprompt'
alias pushd 'pushd \!* && setprompt'
alias popd 'popd \!* && setprompt'
setprompt           # to set the initial prompt
