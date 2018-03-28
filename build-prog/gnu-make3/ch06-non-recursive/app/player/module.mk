local_dir  := app/player
local_pgm  := $(local_dir)/play_mp3
local_src  := $(addprefix $(local_dir)/,play_mp3.c)
local_objs := $(subst .c,.o,$(local_src))

programs   += $(local_pgm)
sources    += $(local_src)

$(local_pgm): $(local_objs) $(libraries)
