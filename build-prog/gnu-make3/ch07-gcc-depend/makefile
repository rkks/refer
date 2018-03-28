SOURCE_DIR := ../ch07-separate-binaries

# $(call source-to-object, source-file-list)
source-to-object = $(subst .c,.o,$(filter %.c,$1)) \
		   $(subst .y,.o,$(filter %.y,$1)) \
		   $(subst .l,.o,$(filter %.l,$1))

# $(subdirectory)
subdirectory = $(patsubst $(SOURCE_DIR)/%/module.mk,%,	\
		 $(word					\
		   $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# $(call make-library, library-name, source-file-list)
define make-library
  libraries += $1
  sources   += $2

  $1: $(call source-to-object,$2)
	$(AR) $(ARFLAGS) $$@ $$^
endef

# $(call generated-source, source-file-list)
generated-source = $(subst .y,.c,$(filter %.y,$1))	\
		   $(subst .y,.h,$(filter %.y,$1))	\
		   $(subst .l,.c,$(filter %.l,$1))

modules      := lib/codec lib/db lib/ui app/player
programs     :=
libraries    :=
sources	     :=

objects      = 	$(call source-to-object,$(sources))
dependencies = 	$(subst .o,.d,$(objects))

include_dirs := lib $(SOURCE_DIR)/lib $(SOURCE_DIR)/include
CPPFLAGS     += $(addprefix -I ,$(include_dirs))
vpath %.h $(include_dirs)

vpath %.y $(SOURCE_DIR)
vpath %.l $(SOURCE_DIR)
vpath %.c $(SOURCE_DIR)

GCC   := gcc
MKDIR := mkdir -p
MV    := mv -f
RM    := rm -f
SED   := sed
TEST  := test

create-output-directories :=				\
	$(shell for f in $(modules);			\
		do					\
		  $(TEST) -d $$f || $(MKDIR) $$f;	\
		done)

all:

include $(patsubst %,$(SOURCE_DIR)/%/module.mk,$(modules))

.PHONY: all
all: $(programs)

.PHONY: libraries
libraries: $(libraries)

.PHONY: clean
clean:
	$(RM) -r *

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(dependencies)
endif

# $(call make-depend,source-file,object-file)
define make-depend
  $(GCC) -MM -MF $(subst .o,.d,$@) -MP -MT $2 $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $1
endef

%.o: %.c
	$(call make-depend,$<,$@)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.c %.h: %.y
	$(YACC.y) --defines $<
	$(MV) y.tab.c $*.c
	$(MV) y.tab.h $*.h
