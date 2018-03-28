PROGS = readline
all: $(PROGS)

readline: readline.c
	$(CC) $(CFLAGS) -o $@ $< -lreadline

.PHONY: clean
clean:
	rm -f $(PROGS) *.o
