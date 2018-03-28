# Find repeated words.  Reads files or standard input.
#
# CHOOSE ONE:
# For Berkeley tr(1):
ww() { cat $* | tr -cs "a-z'" "\012" | uniq -d; }
# For System V tr(1):
#ww() { cat $* | tr -cs "[a-z]'" "[\012*]" | uniq -d; }
