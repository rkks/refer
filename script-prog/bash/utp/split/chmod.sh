# Set and remove write permissions from a file
# Two versions, one for Korn shell (commented out)

# change mode to read only
alias -w='chmod -w'
# change mode to add write permission
alias +w='chmod u+w'

# Korn shell doesn't want to store an alias named '-w', so use these:
#alias c-w='chmod -w'
#alias c+w='chmod u+w'

