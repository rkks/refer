GCC=gcc -W -Wall -ggdb -O0
RM=rm -f

.PHONY: all clean

all: smc

smc: sm.c
	${GCC} -o smc sm.c

clean:
	${RM} *.o smc core
