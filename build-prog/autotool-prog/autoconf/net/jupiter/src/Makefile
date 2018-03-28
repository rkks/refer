all: jupiter

jupiter: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $+

clean:
	-rm jupiter &>/dev/null

check: all
	./jupiter | grep "Hello from .*jupiter!"
	@echo "*** All TESTS PASSED"

install:
	install -d $(DESTDIR)$(bindir)
	install -m 0755 jupiter $(DESTDIR)$(bindir)

uninstall:
	-rm $(DESTDIR)$(bindir)/jupiter &>/dev/null

.PHONY: all clean check install uninstall
