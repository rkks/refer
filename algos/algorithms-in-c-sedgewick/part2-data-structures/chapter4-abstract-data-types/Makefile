CC=gcc -Wall --pedantic-errors
STACKARRAY=STACKarray.c
STACKLIST=STACKlist.c
UF=UF.c
QUEUEARRAY=QUEUEarray.c
QUEUELIST=QUEUElist.c
COMPLEX=COMPLEX.c
COMPLEXFIRSTCLASS=COMPLEXfirstclass.c
QUEUELISTFIRSTCLASS=QUEUEfirstlist.c
POLY=POLY.c

# Section 4.1. Abstract Objects and Collections of Objects
run-exercise01: exercise01.h
	@cat $<
run-exercise02: exercise02.h
	@cat $<

# Section 4.2. Pushdown Stack ADT
run-program01: STACK.h
	@cat $<
run-exercise06: exercise06.md
	@cat $<
run-exercise07: exercise07.md
	@cat $<

# Section 4.3. Examples of Stack ADT Clients
run-program02: program02
	./$< "5 9 8 + 4 6 * * 7 + *"
run-program03: program03
	./$< "( 5 * ( ( ( 9 + 8 ) * ( 4 * 6 ) ) + 7 ) )"
run-exercise09: program03
	./$< "( 5 * ( ( 9 * 8 ) + ( 7 * ( 4 + 6 ) ) ) )"
run-exercise10: exercise10.md
	@cat $<
run-exercise16: exercise16.c
	@cat $<

# Section 4.4. Stack ADT Implementations
run-program04: STACKarray.c
	@cat $<
run-program05: STACKlist.c
	@cat $<
run-exercise18: exercise18
	@echo "If it doesn't work, make sure the source code includes \"ItemChar.h\""
	echo "LA*STI*N*FIR*ST**OU*T*******" | ./$<
run-exercise19: exercise19
	@echo "If it doesn't work, make sure the source files includes \"ItemChar.h\""
	echo "LA*STI*N*FIR*ST**OU*T*******" | ./exercise19-array
	echo "LA*STI*N*FIR*ST**OU*T*******" | ./exercise19-list
run-exercise20: exercise20
	@echo "========================================="
	@echo "If it raises STACKERROR, it does the job."
	@echo "========================================="
	./$<
run-exercise21: exercise21
	@echo "========================================="
	@echo "If it raises STACKERROR, it does the job."
	@echo "========================================="
	./$<

# Section 4.5. Creation of a New ADT
run-program06: UF.h
	@cat $<
run-program07: program07
	echo "3 4\n4 9\n8 0\n2 3\n5 6\n2 9\n5 9\n7 3\n4 8\n5 6\n0 2\n6 1\n" | ./$< 10
run-program08: UF.c
	@cat $<
run-exercise26: exercise26
	./$<
run-exercise29: exercise29
	echo "3 4\n4 9\n8 0\n2 3\n5 6\n2 9\n5 9\n7 3\n4 8\n5 6\n0 2\n6 1\n" | ./$< 10

# Section 4.6. FIFO Queues and Generalized Queues
run-program09: QUEUE.h
	@cat $<
run-program10: QUEUElist.c
	@cat $<
run-program11: QUEUEarray.c
	@cat $<
run-exercise30: exercise30
	echo "FIRS*T*IN***FI*RS***T*OUT*****" | ./$<
run-exercise31: exercise31.md
	@cat $<
run-exercise34: exercise34.md
	@cat $<
run-exercise37: DEQUE.h
	@cat $<
run-exercise38: DEQUEarray.c
	@cat $<
run-exercise41: exercise41
	./$< "EAs+Y+QUE**+st+*+IO*n++*"

# Section 4.7. Duplicate and Index Items
run-program12: STACKunqiue.c
	@cat $<
run-exercise47: exercise47.md
	@cat $<

# Section 4.8. First-Class ADTs
run-program13: program13
	./$< 8
run-program14: COMPLEX.h
	@cat $<
run-program15: COMPLEX.c
	@cat $<
run-program16: COMPLEXfirstclass.h
	@cat $<
run-program17: COMPLEXfirstclass.c
	@cat $<
run-program18: QUEUEfirstclass.h
	@cat $<
run-program19: program19
	./$< 84
run-program20: QUEUEfirstlist.c
	@cat $<
run-exercise57: COMPLEXfirstclass.h
	@tail $< -n 5
	@tail $(COMPLEXFIRSTCLASS) -n 8
run-exercise64: program13
	@./$< 2
	@./$< 3
	@./$< 4
	@./$< 5
	@./$< 6
	@./$< 7
	@./$< 8

# Section 4.9. Application-Based ADT Example
run-program21: program21
	./$< 8 .05
run-program22: POLY.h
	@cat $<
run-program23: POLY.c
	@cat $<

program02: program02-postfix-expression-evaluation.c
	$(CC) -o $@ $< $(STACKARRAY)
program03: program03-infix-to-postfix-conversion.c
	$(CC) -o $@ $< $(STACKLIST)
program07: program07-equivalence-relations-adt-client.c
	$(CC) -o $@ $< $(UF)
program13: program13-complex-numbers-driver-roots-of-unity.c
	$(CC) -o $@ $< $(COMPLEX) -lm
program17: program13-complex-numbers-driver-roots-of-unity.c
	$(CC) -o $@ $< $(COMPLEXFIRSTCLASS) -lm
program19: program19-queue-client-program.c
	$(CC) -o $@ $< $(QUEUELISTFIRSTCLASS)
program21: program21-polynomial-client.c
	$(CC) -o $@ $< $(POLY)

exercise18:
	$(CC) -o $@ $@.c $(STACKARRAY)
exercise19:
	$(CC) -o $@-array $@.c $(STACKARRAY)
	$(CC) -o $@-list $@.c $(STACKLIST)
exercise20:
	$(CC) -o $@ $@.c $(STACKARRAY)
exercise21:
	$(CC) -o $@ exercise20.c $(STACKLIST)
exercise26:
	$(CC) -o $@ $@.c UFhalving.c
exercise29:
	$(CC) -o $@ program07-equivalence-relations-adt-client.c $@.c
exercise30:
	$(CC) -o $@ $@.c $(QUEUEARRAY)
exercise41:
	$(CC) -o $@ $@.c DEQUEarray.c

clean:
	@echo "Cleaning all executable binaries..."
	@$(RM) program02 program03 program07 program13 program17 program19 program21
	@$(RM) exercise18 exercise19-array exercise19-list exercise20 exercise21 exercise26 exercise29 exercise30 exercise41
	@echo "Done."
