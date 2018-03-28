# backwards - a reverse number line
backwards := $(call generate, 9 8 7 6 5 4 3 2 1 0)

# reverse - reverse a list of words
reverse    = $(strip					 \
	       $(foreach f,                              \
                 $(wordlist 1, $(length), $(backwards)), \
                 $(word $f, $1)))

# minus - compute $1 minus $2
minus      = $(word $2,                                \
               $(call reverse,                         \
                 $(wordlist 1, $1, $(number_line))))

minus:
	# $(call minus, 7, 4)
