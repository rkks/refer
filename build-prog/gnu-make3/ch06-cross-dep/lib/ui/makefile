lib_ui 	     := libui.a
sources      := ui.c
objects      := $(subst .c,.o,$(sources))
dependencies := $(subst .c,.d,$(sources))

include_dirs = .. ../../include
vpath %.h $(include_dirs)
CPPFLAGS += $(addprefix -I ,$(include_dirs))

all: $(lib_ui)

$(lib_ui): $(objects)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: clean
clean:
	$(RM) $(lib_ui) $(objects) $(dependencies)

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< |	\
	sed 's,\($*\.o\) *:,\1 $@: ,' > $@.tmp
	mv $@.tmp $@
