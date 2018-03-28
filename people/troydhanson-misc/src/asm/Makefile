EXE = hello atoi gethost addc penv
all: $(EXE)

# make has an implicit rule to expect the targets like hello to
# depend on hello.o; and here we define our own implicit rule
# so that .o depends on .asm

%.o: %.asm
	nasm -f elf $<

addc: addc.o add.o

.PHONY: clean

clean:
	rm -f *.o $(EXE)
