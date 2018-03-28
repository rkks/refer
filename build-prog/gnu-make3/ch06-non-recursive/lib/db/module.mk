local_dir   := lib/db
local_lib   := $(local_dir)/libdb.a
local_src   := $(addprefix $(local_dir)/,scanner.c playlist.c)
local_objs  := $(subst .c,.o,$(local_src))

libraries   += $(local_lib)
sources     += $(local_src)
extra_clean += $(local_src) $(local_dir)/playlist.h

$(local_lib): $(local_objs)
	$(AR) $(ARFLAGS) $@ $^

.SECONDARY: $(extra_clean)
