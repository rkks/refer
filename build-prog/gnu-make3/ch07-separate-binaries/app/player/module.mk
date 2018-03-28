local_pgm  := $(subdirectory)/play_mp3
local_src  := $(subdirectory)/play_mp3.c
local_objs := $(call source-to-object,$(local_src))

programs   += $(local_pgm)
sources    += $(local_src)

$(local_pgm): $(local_objs) $(libraries)
