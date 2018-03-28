lib_db       := libdb.a
sources      := scanner.c playlist.c
objects      := $(subst .c,.o,$(sources))
dependencies := $(subst .c,.d,$(sources))

include_dirs = .. ../../include
vpath %.h $(include_dirs)
CPPFLAGS += $(addprefix -I ,$(include_dirs))

all: $(lib_db)

$(lib_db): $(objects)
	$(AR) $(ARFLAGS) $@ $^

playlist.o: db/playlist.h
scanner.o: db/playlist.h

.SECONDARY: $(sources) playlist.h

.PHONY: clean
clean:
	$(RM) $(lib_db) $(objects) $(dependencies) $(sources) playlist.h

%.c %.h: %.y
	$(YACC.y) --defines $<
	mv -f y.tab.c $*.c
	mv -f y.tab.h $*.h

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -M $< |	\
	sed 's,\($*\.o\) *:,\1 $@: ,' > $@.tmp
	mv $@.tmp $@
