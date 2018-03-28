SHELL := /bin/bash
export TIMEFORMAT = %2R

# Ten character file
dir := ab/cd/ef/g
# 1000 character path
p100 := $(dir);$(dir);$(dir);$(dir);$(dir);$(dir);$(dir);$(dir);$(dir);x
p1000 := $(p100)$(p100)$(p100)$(p100)$(p100)$(p100)$(p100)$(p100)$(p100)$(p100)

# A sequence of 100 words
w100 := $(foreach x,0 1 2 3 4 5 6 7 8 9,\
	  $(foreach y,0 1 2 3 4 5 6 7 8 9,\
	    $x$y))

export dir p1000 w100 TMAKE

define ten-times
  TESTS += $1
  .PHONY: $1
  $1:
	@echo $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2; \
	time $(MAKE) --no-print-directory --silent $2
endef

.PHONY: all
all:

$(eval $(call ten-times, make-bash, -f make-bash.mk))
$(eval $(call ten-times, make-ash, -f make-ash.mk))
$(eval $(call ten-times, make-sh, -f make-sh.mk))
$(eval $(call ten-times, assign, -q -f assign.mk))
$(eval $(call ten-times, subst-short, -q -f subst-short.mk))
$(eval $(call ten-times, subst-long, -q -f subst-long.mk))
$(eval $(call ten-times, sed-bash, -f sed-bash.mk))
$(eval $(call ten-times, sed-ash, -f sed-ash.mk))
$(eval $(call ten-times, sed-sh, -f sed-sh.mk))
$(eval $(call ten-times, shell-bash, -f shell-bash.mk))
$(eval $(call ten-times, shell-ash, -f shell-ash.mk))
$(eval $(call ten-times, shell-sh, -f shell-sh.mk))

all: $(TESTS)
