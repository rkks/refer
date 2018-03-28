SOURCE_DIR := /c/home/mecklen/book/examples/ch07-separate-binaries-1
BINARY_DIR := /c/home/mecklen/book/out/mp3_player_out

# $(call source-dir-to-binary-dir, directory-list)
source-dir-to-binary-dir = $(addprefix $(BINARY_DIR)/, $1)

# $(call source-to-object, source-file-list)
source-to-object = $(call source-dir-to-binary-dir,	\
		     $(subst .c,.o,$(filter %.c,$1))	\
		     $(subst .y,.o,$(filter %.y,$1))	\
		     $(subst .l,.o,$(filter %.l,$1)))

# $(subdirectory)
subdirectory = $(patsubst %/module.mk,%,		\
		 $(word					\
		   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# $(call make-library, library-name, source-file-list)
define make-library
  libraries += $(BINARY_DIR)/$1
  sources   += $2

  $(BINARY_DIR)/$1: $(call source-dir-to-binary-dir,	\
		      $(subst .c,.o,$(filter %.c,$2))	\
		      $(subst .y,.o,$(filter %.y,$2))	\
		      $(subst .l,.o,$(filter %.l,$2)))
	$(AR) $(ARFLAGS) $$@ $$^
endef

# $(call generated-source, source-file-list)
generated-source = $(call source-dir-to-binary-dir,	\
		     $(subst .y,.c,$(filter %.y,$1))	\
		     $(subst .y,.h,$(filter %.y,$1))	\
		     $(subst .l,.c,$(filter %.l,$1)))	\
		   $(filter %.c,$1)

# $(compile-rules)
define compile-rules
  $(foreach f, $(local_src),\
    $(call one-compile-rule,$(call source-to-object,$f),$f))
endef

# $(call one-compile-rule, binary-file, source-files)
define one-compile-rule
  $1: $(call generated-source,$2)
	$(COMPILE.c) -o $$@ $$<

  $(subst .o,.d,$1): $(call generated-source,$2)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $$< | \
	$(SED) 's,\($$(notdir $$*)\.o\) *:,$$(dir $$@)\1 $$@: ,' > $$@.tmp
	$(MV) $$@.tmp $$@

endef

modules      := lib/codec lib/db lib/ui app/player
programs     :=
libraries    :=
sources	     :=

objects      = 	$(call source-to-object,$(sources))
dependencies = 	$(subst .o,.d,$(objects))

include_dirs := $(BINARY_DIR)/lib lib include
CPPFLAGS     += $(addprefix -I ,$(include_dirs))
vpath %.h $(include_dirs)

MKDIR := mkdir -p
MV    := mv -f
RM    := rm -f
SED   := sed
TEST  := test

create-output-directories :=						\
	$(shell for f in $(call source-dir-to-binary-dir,$(modules));	\
		do							\
		  $(TEST) -d $$f || $(MKDIR) $$f;			\
		done)

all:

include $(addsuffix /module.mk,$(modules))

.PHONY: all
all: $(programs)

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	$(RM) -r $(BINARY_DIR)

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif
