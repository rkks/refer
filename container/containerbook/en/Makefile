SUBDIRS := $(shell find . -name code-samples -type d)

.PHONY: force

all: $(SUBDIRS)

examples:
	for dir in $(SUBDIRS); do      \
	    $(MAKE) -C $$dir examples; \
	    if [ $$? -ne 0 ]; then     \
		exit 1;                \
	    fi                         \
	done

examples_clean:
	for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir examples_clean; \
	done
