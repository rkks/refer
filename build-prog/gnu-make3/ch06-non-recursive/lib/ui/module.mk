local_dir  := lib/ui
local_lib  := $(local_dir)/libui.a
local_src  := $(addprefix $(local_dir)/,ui.c)
local_objs := $(subst .c,.o,$(local_src))

libraries  += $(local_lib)
sources    += $(local_src)

$(local_lib): $(local_objs)
	$(AR) $(ARFLAGS) $@ $^

$(local_dir)/ui.d: lib/db/playlist.h
