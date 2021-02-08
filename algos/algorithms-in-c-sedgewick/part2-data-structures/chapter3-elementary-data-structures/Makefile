CC=gcc -Wall
CFLAGS=-O2 -Wall -pedantic-errors
MATHFLAG=-lm

# Section 3.1. Building Blocks
run-program01: program01
	./$<
run-program02: program02
	./$< 300
run-exercise01: exercise01
	./$<
run-exercise02: exercise02
	./$< 10 1000
	./$< 10 10000
	./$< 10 100000
	./$< 10 1000000
	./$< 1000 1000
	./$< 1000 10000
	./$< 1000 100000
	./$< 1000 1000000
run-exercise03: exercise03
	./$< 10 1000
	./$< 10 10000
	./$< 10 100000
	./$< 10 1000000
	./$< 1000 1000
	./$< 1000 10000
	./$< 1000 100000
	./$< 1000 1000000
run-exercise04: exercise02 exercise03
	./exercise02 2 1000000
	./exercise02 4 1000000
	./exercise02 16 1000000
	./exercise03 2 1000000
	./exercise03 4 1000000
	./exercise03 16 1000000
run-exercise05: exercise05
	./$< 1000000
run-exercise07: exercise07
	echo "10 10\n15 100\n3.5 13.5\n1 1\n500 12" | ./$<

# Section 3.2. Arrays
run-program05: program05
	./$<
run-program06: program06
	./$< 100
run-program07: program07
	./$< 32 1000
run-program08: program08
	./$< 1000 .5
run-exercise11: exercise11
	./$<
run-exercise12: exercise12
	./$<
run-exercise13: exercise13
	./$<
run-exercise17: exercise17
	shuf -i 1-10000 -n 10 | ./$<
run-exercise20: exercise20
	./$< 32 1000 0.166666
run-exercise21: exercise21
	./$< 32 1000 20

# Section 3.3. Linked Lists
run-program09: program09
	./$< 9 5
run-exercise24: exercise24
	./$< 9 5
run-exercise25: exercise25
	./$< 9 5
run-exercise26: exercise26
	./$<
run-exercise27: exercise27
	./$<
run-exercise28: exercise28
	./$< 9 5

# Section 3.4. Elementary List Processing
run-program10: program10
	./$<
run-program11: program11
	./$<
run-table01: table01-head-and-tail-conventions-in-linked-lists.md
	cat $<
run-program13: program13
	./$< 9 5
run-exercise34: exercise34
	./$<

# Section 3.5. Memory Allocation for Lists

# Section 3.6. Strings
run-table02: table02-elementary-string-processing-operations.md
	cat $<
run-program15: program15
	./$< string < program15-string-search.c
run-exercise56: exercise56
	./$< "hello world here comes to sun"
run-exercise57: exercise57
	@./$< "if i had a hifi"
	@./$< "whatever"
run-exercise59: exercise59
	./$< "string" < $<.c
run-exercise61: exercise61
	./$< "string" < program15-string-search.c

# Section 3.7. Compound Data Structures
run-program16: program16
	./$<
run-program17: program17
	./$< < program17-sorting-an-array-of-strings.c
run-program18: program18
	./$< < graph-links.txt
run-program19: program19
	./$< < graph-links.txt


# ===
program01: program01-function-definition.c
	$(CC) $(CFLAGS) -o $@ $<
program02: program02-types-of-numbers.c
	$(CC) $(CFLAGS) -o $@ $< $(MATHFLAG)
program05: program05-sieve-for-eratosthenes.c
	$(CC) $(CFLAGS) -o $@ $<
program06: program06-dynamic-memory-allocation-for-array.c
	$(CC) $(CFLAGS) -o $@ $<
program07: program07-coin-flipping-simulation.c
	$(CC) $(CFLAGS) -o $@ $<
program08: program08-closest-point-computation.c
	$(CC) $(CFLAGS) -o $@ $< Point.c $(MATHFLAG)
program09: program09-circular-list-example-josephus-problem.c
	$(CC) $(CFLAGS) -o $@ $<
program10: program10-list-reversal.c
	$(CC) $(CFLAGS) -o $@ $<
program11: program11-list-insertion-sort.c
	$(CC) $(CFLAGS) -o $@ $<
program13: program13-list-allocation-for-josephus.c
	$(CC) $(CFLAGS) -o $@ $< list.c
program15: program15-string-search.c
	$(CC) $(CFLAGS) -o $@ $<
program16: program16-two-dimensional-array-allocation.c
	$(CC) $(CFLAGS) -o $@ $<
program17: program17-sorting-an-array-of-strings.c
	$(CC) -o $@ $<
program18: program18-adjacency-matrix-graph-representation.c
	$(CC) $(CFLAGS) -o $@ $<
program19: program19-adjacency-lists-graph-representation.c
	$(CC) $(CFLAGS) -o $@ $<

exercise02: exercise02.c
	$(CC) $(CFLAGS) -o $@ $< $(MATHFLAG)
exercise03: exercise03.c
	$(CC) $(CFLAGS) -o $@ $< $(MATHFLAG)
exercise05: exercise05.c
	$(CC) $(CFLAGS) -o $@ $< $(MATHFLAG)
exercise07: exercise07.c
	$(CC) $(CFLAGS) -o $@ $< Point.c $(MATHFLAG)
exercise13: sieve.c
	$(CC) $(CFLAGS) -o $@ $@.c $<
exercise24: linkedlist.c
	$(CC) $(CFLAGS) -o $@ $@.c $<
exercise25: linkedlist.c
	$(CC) $(CFLAGS) -o $@ $@.c $<
exercise26: linkedlist.c
	$(CC) $(CFLAGS) -o $@ $@.c $<
exercise27: linkedlist.c
	$(CC) $(CFLAGS) -o $@ $@.c $<
exercise34: list.c
	$(CC) $(CFLAGS) -o $@ $@.c $<

clean:
	@echo "Cleaning all executable binaries..."
	@$(RM) program01 program02 program05 program06 program07 program08 program09 program10 program11 program13 program15 program16 program17 program18 program19
	@$(RM) exercise01 exercise02 exercise03 exercise05 exercise07 exercise11 exercise12 exercise13 exercise17 exercise20 exercise21 exercise24 exercise25 exercise26 exercise27 exercise28 exercise34 exercise56 exercise57 exercise59 exercise61
	@echo "Done."
