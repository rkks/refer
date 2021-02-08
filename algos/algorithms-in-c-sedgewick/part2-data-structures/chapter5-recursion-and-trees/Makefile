CC=gcc -Wall --pedantic-errors

# Section 5.1. Recursive Algorithms
run-program01: program01-factorial-function.c
	@cat $<
run-program02: program02-a-questionable-recursive-program.c
	@cat $<
run-program03: program03-euclid-algorithm.c program03
	@cat $<
	./program03 314159 271828
run-program04: program04-recursive-program-to-evaluate-prefix-expressions.c program04
	@cat $<
	./program04 "* + 7 * * 4 6 + 8 9 5"
run-program05: program05-recursive-functions-for-linked-lists.c
	@cat $<
run-exercise01: exercise01
	./$< 100
run-exercise02: exercise02
	./exercise02 1000 997
	./exercise02 10000 997
	./exercise02 100000 997
	./exercise02 1000000 997
run-exercise05: exercise05
	./$< 314159 271828
run-exercise06: exercise06
	./$< 89 55
run-exercise08: exercise08
	./$< "+ * * 12 12 12 144"

# Section 5.2. Divide and Conquer
run-program06: program06-divide-and-conquer-to-find-the-maximum.c
	@cat $<
run-program07: program07-towers-of-hanoi.c
	@cat $<
run-program08: program08-divide-and-conquer-to-draw-a-ruler.c
	@cat $<
run-program09: program09-nonrecursive-program-to-draw-a-ruler.c
	@cat $<
run-exercise16: exercise16
	./$< 1 2 3 4 5 6 7 8 9 44234

# Section 5.3. Dynamic Programming
run-program10: program10-fibonacci-numbers-recursive.c
	@cat $<
run-program11: program11-fibonacci-numbers-dp.c
	@cat $<
run-program12: program12-knapsack-problem-recursive.c
	@cat $<
run-program13: program13-knapsack-problem-dp.c
	@cat $<
run-exercise40: exercise40
	./$<
run-exercise41: exercise41
	./$<
run-exercise44: exercise44
	./$< 20
run-exercise45: exercise45
	./$< 20
run-exercise55: exercise55
	./$< 10 4

# Section 5.4. Trees
run-exercise63: exercise63.md
	@cat $<

# Section 5.5. Mathematical Properties of Binary Trees
# Section 5.6. Tree Traversal
run-program14: program14-recursive-tree-traversal.c
	@cat $<
run-program15: program15-preorder-traversal-nonrecursive.c
	@cat $<
run-program16: program16-level-order-traversal.c
	@cat $<

# Section 5.7. Recursive Binary-Tree Algorithms
run-program17: program17-computation-of-tree-parameters.c
	@cat $<
run-program18: program18-quick-tree-print-function.c
	@cat $<
run-program19: program19-construction-of-a-tournament.c
	@cat $<
run-program20: program20-construction-of-a-parse-tree.c
	@cat $<

# Section 5.8. Graph Traversal
run-program21: program21-depth-first-search.c
	@cat $<
run-program22: program22-breadth-first-search.c
	@cat $<

program03: program03-euclid-algorithm.c
	$(CC) -o $@ $<
program04: program04-recursive-program-to-evaluate-prefix-expressions.c
	$(CC) -o $@ $<

exercise01: exercise01.c
	$(CC) -o $@ $< -lm
exercise40: exercise40.c
	$(CC) -o $@ $< -lm
exercise41: exercise41.c
	$(CC) -o $@ $< -lm

clean:
	@echo "Cleaning all executable binaries..."
	@$(RM) program03 program04
	@$(RM) exercise01 exercise02 exercise05 exercise06 exercise08 exercise16 exercise40 exercise41 exercise44 exercise45 exercise55
	@echo "Done."
