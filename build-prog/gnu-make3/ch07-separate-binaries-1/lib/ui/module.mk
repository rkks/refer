local_src := $(subdirectory)/ui.c

$(eval $(call make-library,$(subdirectory)/libui.a,$(local_src)))

$(eval $(compile-rules))

$(BINARY_DIR)/$(subdirectory)/ui.d: $(BINARY_DIR)/lib/db/playlist.h
