PROGS = looper looper_readline
all: $(PROGS)

looper: looper.c

looper_readline: looper_readline.c
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

.PHONY: clean
clean:
	rm -f $(PROGS) *.o

