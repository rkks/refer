$(warning A top-level warning)
FOO := $(warning Right-hand side of a simple variable)bar
BAZ = $(warning Right-hand side of a recursive variable)boo

$(warning A target)target: $(warning In a prerequisite list)makefile $(BAZ)
	$(warning In a command script)
	ls
$(BAZ):
