CFLAGS=-Wall

all: program-a program-b

clean:
	-rm -f *.o *~ program-a program-b

program-a: program-a.o
	$(CC) program-a.o -o $@
	
program-b: program-b.o
	$(CC) program-a.o -o $@
	
