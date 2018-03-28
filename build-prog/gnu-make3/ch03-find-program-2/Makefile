# $(call find-program,wildcard-pattern)
find-program = $(wildcard			\
                 $(addsuffix /$1,		\
		   $(sort			\
                     $(subst :, ,		\
                       $(subst ::,:.:,		\
                         $(patsubst :%,.:%,	\
                           $(patsubst %:,%:.,$(PATH))))))))
find:
	@echo $(words $(call find-program,*))
