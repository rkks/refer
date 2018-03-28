program      := play_mp3
sources      := play_mp3.c
objects      := $(subst .c,.o,$(sources))
libraries    := -ldb -lcodec -lui
dependencies := $(subst .c,.d,$(sources))

include_dirs := .. ../../include
CPPFLAGS     += $(addprefix -I , $(include_dirs))
vpath %.h $(include_dirs)

lib_dirs     := ../../lib/codec ../../lib/db ../../lib/ui
vpath %.a $(lib_dirs)

all: $(program)

$(program): $(objects) $(libraries)

.PHONY: clean
clean:
	$(RM) $(program) $(objects) $(dependencies)

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< |	\
	sed 's,\($*\.o\) *:,\1 $@: ,' > $@.tmp
	mv $@.tmp $@
