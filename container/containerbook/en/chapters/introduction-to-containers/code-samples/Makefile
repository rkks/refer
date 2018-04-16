.PHONY: clean

all: examples

helloworld:
	$(CC) -static helloworld.c -o helloworld

contejner:
	$(CC) contejner.c -o contejner

examples: contejner helloworld

examples_clean:
	rm -f contejner helloworld
