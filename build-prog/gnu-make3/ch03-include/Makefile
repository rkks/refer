# Simple makefile including a generated file.
include foo.mk
$(warning Finished include)

foo.mk: bar.mk
	m4 --define=FILENAME=$@ bar.mk > $@
