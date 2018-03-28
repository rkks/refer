# Show repeated words.  Reads from files or standard input.
#
# CHOOSE ONE:
# For Berkeley tr(1):
alias ww 'cat \!* | tr -cs "a-z'\''" "\012" | uniq -d'
# For System V tr(1):
#alias ww 'cat \!* | tr -cs "[a-z]'\''" "[\012*]" | uniq -d'
