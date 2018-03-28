SUBDIRS=base sine play

all: $(SUBDIRS)

$(SUBDIRS) doc:
	$(MAKE) -C $@


clean: 
	for f in $(SUBDIRS) doc; do make -C $$f $@; done

.PHONY: clean $(SUBDIRS)
