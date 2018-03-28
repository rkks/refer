# quote and makealias -- Get quoting right in csh aliases

alias quote     "/bin/sed -e 's/\\!/\\\\\!/g' -e 's/'\\\''/'\\\'\\\\\\\'\\\''/g' -e 's/^/'\''/' -e 's/"\$"/'\''/'"
alias makealias "quote | /bin/sed 's/^/alias \!:1 /' \!:2*"
