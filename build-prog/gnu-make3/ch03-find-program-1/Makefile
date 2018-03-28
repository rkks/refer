# $(call find-program, filter-pattern)
find-program = $(filter $1,			\
                 $(wildcard			\
                   $(addsuffix /*,		\
		     $(sort			\
                       $(subst :, ,		\
                         $(subst ::,:.:,	\
                           $(patsubst :%,.:%,	\
                             $(patsubst %:,%:.,$(PATH)))))))))
find:
	@echo $(words $(call find-program, %))
