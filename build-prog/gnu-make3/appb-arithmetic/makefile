# combine - concatentate one sequence of numbers with another
combine = $(foreach i, $1, $(addprefix $i, $2))

# stripzero - Remove one leading zero from each word
stripzero = $(patsubst 0%,%,$1)

# generate - Produce all permutations of three elements from the word list
generate = $(call stripzero,				\
             $(call stripzero,				\
               $(call combine, $1,			\
	         $(call combine, $1, $1))))

# number_line - Create a number line from 0 to 999
number_line := $(call generate,0 1 2 3 4 5 6 7 8 9)
length      := $(word $(words $(number_line)), $(number_line))

# plus - Use the number line to add two integers
plus = $(word $2,					\
         $(wordlist $1, $(length),			\
	   $(wordlist 3, $(length), $(number_line))))

# gt - Use the number line to determine if $1 is greater than $2
gt = $(filter $1,					\
       $(wordlist 3, $(length),				\
	 $(wordlist $2, $(length), $(number_line))))


all:
	@echo $(call plus,4,7)
	@echo $(if $(call gt,4,7),is,is not)
	@echo $(if $(call gt,7,4),is,is not)
	@echo $(if $(call gt,7,7),is,is not)
