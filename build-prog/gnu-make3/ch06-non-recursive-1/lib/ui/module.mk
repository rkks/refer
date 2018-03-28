local_src := $(wildcard $(subdirectory)/*.c)

$(eval $(call make-library, $(subdirectory)/libui.a, $(local_src)))

$(subdirectory)/ui.d: lib/db/playlist.h
