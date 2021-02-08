CC=cc
MATHFLAG=-lm

# Section 6.1. Rules of the Game
run-program01: program01
	./$< 20 1

run-exercise01: exercise01.md
	@cat $<

run-exercise02: exercise02.md
	@cat $<

run-exercise05: exercise05.md
	@cat $<

run-exercise06: exercise06.md
	@cat $<

program01: program01-example-array-sort-with-driver.c program01-insertion-sort.c
	$(CC) -o $@ program01-example-array-sort-with-driver.c program01-insertion-sort.c

# Section 6.2. Selection Sort
run-program02: program02
	./$< 20 1

run-exercise11: exercise11.md
	@cat $<

program02: program01-example-array-sort-with-driver.c program02-selection-sort.c
	$(CC) -o $@ program01-example-array-sort-with-driver.c program02-selection-sort.c

# Section 6.3. Insertion Sort
run-program03: program03
	./$< 20 1

run-exercise15: exercise15.md
	@cat $<

run-exercise16: exercise16
	./$< 20 1

program03: program01-example-array-sort-with-driver.c program03-insertion-sort.c
	$(CC) -o $@ program01-example-array-sort-with-driver.c program03-insertion-sort.c

# Section 6.4. Bubble Sort
run-program04: program04
	./$< 20 1

run-exercise20: exercise20.md
	@cat $<

program04: program01-example-array-sort-with-driver.c program04-bubble-sort.c
	$(CC) -o $@ program01-example-array-sort-with-driver.c program04-bubble-sort.c

# Section 6.6. Shellsort
run-program05: program05
	./$< 20 1

program05: program01-example-array-sort-with-driver.c program05-shellsort.c
	$(CC) -o $@ program01-example-array-sort-with-driver.c program05-shellsort.c

# Section 6.7. Sorting of Other Types of Data
run-program06: program06-sort-driver-for-arrays.c
	@cat $<

run-program07: program07-interface-for-array-data-type.h
	@cat $<

run-program08: program08-implementation-of-array-data-type.c
	@cat $<

run-program09: program09-sample-interface-for-item-data-type.h
	@cat $<

run-program10: program10-sample-implementation-for-item-data-type.c
	@cat $<

run-section-6: program-double-selection program-double-insertion program-double-bubble program-double-shellsort
	@echo "Selection sort for double data type"
	./program-double-selection 20 1
	@echo "Insertion sort for double data type"
	./program-double-insertion 20 1
	@echo "Bubble sort for double data type"
	./program-double-bubble 20 1
	@echo "Shellsort for double data type"
	./program-double-shellsort 20 1

run-exercise49: exercise49-long-selection exercise49-long-insertion exercise49-long-bubble exercise49-long-shellsort
	@echo "Selection sort for long data type"
	./exercise49-long-selection 20 1
	@echo "Insertion sort for long data type"
	./exercise49-long-insertion 20 1
	@echo "Bubble sort for long data type"
	./exercise49-long-bubble 20 1
	@echo "Shellsort for long data type"
	./exercise49-long-shellsort 20 1

run-exercise52: exercise52
	./$< 20 1

program-double-selection: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c program09-sample-interface-for-item-data-type.h program10-sample-implementation-for-item-data-type.c selection-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp program09-sample-interface-for-item-data-type.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c program10-sample-implementation-for-item-data-type.c selection-sort.c

program-double-insertion: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c program09-sample-interface-for-item-data-type.h program10-sample-implementation-for-item-data-type.c insertion-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp program09-sample-interface-for-item-data-type.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c program10-sample-implementation-for-item-data-type.c insertion-sort.c

program-double-bubble: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c program09-sample-interface-for-item-data-type.h program10-sample-implementation-for-item-data-type.c bubble-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp program09-sample-interface-for-item-data-type.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c program10-sample-implementation-for-item-data-type.c bubble-sort.c

program-double-shellsort: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c program09-sample-interface-for-item-data-type.h program10-sample-implementation-for-item-data-type.c shellsort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp program09-sample-interface-for-item-data-type.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c program10-sample-implementation-for-item-data-type.c shellsort.c

exercise49-long-selection: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c exercise49-long-int.h exercise49-long-int.c selection-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp exercise49-long-int.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c exercise49-long-int.c selection-sort.c

exercise49-long-insertion: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c exercise49-long-int.h exercise49-long-int.c insertion-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp exercise49-long-int.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c exercise49-long-int.c insertion-sort.c

exercise49-long-bubble: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c exercise49-long-int.h exercise49-long-int.c bubble-sort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp exercise49-long-int.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c exercise49-long-int.c bubble-sort.c

exercise49-long-shellsort: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c exercise49-long-int.h exercise49-long-int.c shellsort.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp exercise49-long-int.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c exercise49-long-int.c shellsort.c

exercise52: program06-sort-driver-for-arrays.c exercise52-check.h exercise52-check.c program09-sample-interface-for-item-data-type.h program10-sample-implementation-for-item-data-type.c shellsort.c
	@cp exercise52-check.h Array.h
	@cp program09-sample-interface-for-item-data-type.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c exercise52-check.c program10-sample-implementation-for-item-data-type.c shellsort.c

# Section 6.8. Index and Pointer Sorting
run-exercise57: exercise57
	./$< 6 0 < records.txt

exercise57: program06-sort-driver-for-arrays.c program07-interface-for-array-data-type.h program08-implementation-of-array-data-type.c program12-data-type-interface-for-record-items.h program13-data-type-implementation-for-record-items.c program13-less.c
	@cp program07-interface-for-array-data-type.h Array.h
	@cp program12-data-type-interface-for-record-items.h Item.h
	$(CC) -o $@ program06-sort-driver-for-arrays.c program08-implementation-of-array-data-type.c program13-data-type-implementation-for-record-items.c program13-less.c

clean:
	@echo "Cleaning all executable binaries..."
	@$(RM) program01 program02 program03 program04 program05 program-double-selection program-double-insertion program-double-bubble program-double-shellsort
	@$(RM) exercise16 exercise49-long-selection exercise49-long-insertion exercise49-long-bubble exercise49-long-shellsort exercise52 exercise57
	@$(RM) Array.h Item.h
	@echo "Done."
