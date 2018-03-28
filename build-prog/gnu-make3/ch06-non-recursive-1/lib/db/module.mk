local_src := $(wildcard $(subdirectory)/*.[ly])

$(eval $(call make-library, $(subdirectory)/libdb.a, $(local_src)))

.SECONDARY: $(call generated-source, $(local_src))
