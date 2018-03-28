YFLAGS = -d
OBJS = hoc.o lex.o init.o math.o symbol.o

hoc3:	$(OBJS)
	cc $(OBJS) -lm -ll -o hoc3

hoc.o:  hoc.h

lex.o init.o symbol.o:	hoc.h y.tab.h
