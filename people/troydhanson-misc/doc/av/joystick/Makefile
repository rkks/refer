EXES=joystick
all:$(EXES)

$(EXES): %: %.c
	$(CC) $(CFLAGS) -o $@ $< -lSDL

clean:
	rm -f $(EXES) *.o
