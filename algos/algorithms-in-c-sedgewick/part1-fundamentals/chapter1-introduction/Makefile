CC=gcc
CFLAGS=-O2 -Wall -pedantic-errors

# Section 1.1. Algorithms
# Section 1.2. A Sample Problem: Connectivity
run-exercise01: exercise01.md
	cat $<

# Section 1.3. Union-Find Algorithms
run-figure03: figure03
	./$<
run-program01: program01
	echo "3 4\n4 9\n8 0\n2 3\n5 6\n2 9\n5 9\n7 3\n4 8\n5 6\n0 2\n6 1\n" | ./$<
run-program02: program02
	echo "3 4\n4 9\n8 0\n2 3\n5 6\n2 9\n5 9\n7 3\n4 8\n5 6\n0 2\n6 1\n5 8\n" | ./$<
run-figure06: figure06
	./$<
run-program03: program03
	echo "0 1\n2 3\n4 5\n6 7\n8 9\n2 1\n6 5\n4 3\n8 7\n" | ./$<
run-program04: program04
	echo "0 1\n2 1\n3 2\n4 3\n5 4\n6 5\n7 6\n8 7\n" | ./$<
run-exercise04: exercise04
	./$<
run-exercise05: exercise05
	./$<
run-exercise06: exercise06
	./$<
run-exercise07: exercise07
	./$<
run-exercise08: exercise08
	./$<
run-exercise14: exercise14
	./$<
run-exercise15: exercise15
	./$<
run-exercise16: exercise16
	echo "3 4\n4 9\n8 0\n2 3\n5 6\n2 9\n5 9\n7 3\n4 8\n5 6\n0 2\n6 1\n" | ./$<
run-exercise17: exercise17
	./$<

# Section 1.4. Perspective
# Section 1.5. Summary of Topics

# ===
figure03: figure03-quickfind-example.c
	$(CC) $(CFLAGS) -o $@ $<
figure06: figure06-quickunion-example.c
	$(CC) $(CFLAGS) -o $@ $<
program01: program1-quickfind.c
	$(CC) $(CFLAGS) -o $@ $<
program02: program2-quickunion.c
	$(CC) $(CFLAGS) -o $@ $<
program03: program3-weighted-quick-union.c
	$(CC) $(CFLAGS) -o $@ $<
program04: program4-weighted-quick-union-with-path-compression-by-halving.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo "Cleaning all executable binaries..."
	@$(RM) figure03 figure06
	@$(RM) program01 program02 program03 program04
	@$(RM) exercise04 exercise05 exercise06 exercise07 exercise08 exercise14 exercise15 exercise16 exercise17
	@echo "Done."
