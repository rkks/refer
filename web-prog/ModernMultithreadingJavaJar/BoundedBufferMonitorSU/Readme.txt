Program BoundedBufferMonitorSU is a semaphore solution to the bounded
buffer problem with 3 producer, 3 consumers, and a 2-slot buffer.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU.java

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU 
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU 

This will produce 6 files:
- ThreadID.txt: Thread IDs that were generated 
- conditionVarID.txt : The conditionVariables names that were used
- monitorID.txt: The monitor name that was used
- monitor-replay.txt: simple M-sequence collected during execution
- monitor-test.txt: M-sequence collected during execution 
- monitor-spectest.txt: communication-sequence collected during execution

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU 
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU

To see what happens when there is a deadlock, you can change the if-statement in deposit()
and withdraw() to:

   if (true) ...

so that all the producer and consumer threads will block themselves. Run the program in trace 
mode with deadlockDetection on and you will see:

  Monitoring for Deadlock.
  Deadlock detected:
  - Consumer2 blocked on waitC() in method Buffer:withdraw
  - Producer2 blocked on waitC() in method Buffer:deposit
  - Producer3 blocked on waitC() in method Buffer:deposit
  - Consumer3 blocked on waitC() in method Buffer:withdraw
  - Producer1 blocked on waitC() in method Buffer:deposit
  - Consumer1 blocked on waitC() in method Buffer:withdraw
 
  Execution trace:
  Thread Producer3 entered monitor method deposit
  Thread Producer3 blocked on notFull.waitC() in Buffer:deposit
  Thread Consumer1 entered monitor method withdraw
  Thread Consumer1 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Consumer3 entered monitor method withdraw
  Thread Consumer3 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Producer2 entered monitor method deposit
  Thread Producer2 blocked on notFull.waitC() in Buffer:deposit
  Thread Consumer2 entered monitor method withdraw
  Thread Consumer2 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Producer1 entered monitor method deposit
  Thread Producer1 blocked on notFull.waitC() in Buffer:deposit

*** Make sure you change the if-statement back to the way it was and recompile the program.
    Run it again in TRACE mode to collect a non-deadlock sequence so we can replay it below. ***

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at monitor entry.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU 
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU 

When the sequence in semaphore-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU 
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU 

This will determine whether the M-sequence in monitor-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in monitor-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put M-sequences of your own in monitor-test.txt and
determine whether they are feasible.

5. Run it in spectest mode:
   Windows:   java -Dmode=spectest -classpath .;../ModernMultithreading.jar  BoundedBufferMonitorSU 
   Unix:      java -Dmode=spectest -classpath .:../ModernMultithreading.jar  BoundedBufferMonitorSU 

This will determine whether the communication-sequence in monitor-spectest.txt is feasible; 
you will see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in monitor-spectest.txt was collected when we traced the execution, so it is 
definitely feasible. You can put communication-sequences of your own in monitor-spectest.txt 
and determine whether they are feasible.

For example, change the first event in monitor-spectest.txt to be:

  (4,withdraw)

and run it in spectest mode again. Since no sequence can start with a withdrawl by a consumer,
this sequence is infeasible. You will see: 
   Infeasible sequence - timeout waiting for event 1: (4, withdraw).

6. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferMonitorSU
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferMonitorSU

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Fri Nov 18 13:29:30 GMT-05:00 2005
1/1
25/25
50/50
75/75
100/100
125/125
150/150
175/175
200/200
225/225
250/250
275/275
300/300
325/325
350/350
375/375
400/400
425/425
450/450
475/475
500/500
525/525
550/550
575/575
600/600
625/625
650/650
675/675
700/700

Reachability Testing completed.
  Executions:720 / Sequences Collected:720
  Elapsed time in minutes: 0.2
  Elapsed time in seconds: 12.0
  Elapsed time in milliseconds: 12000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

You can also run RT mode with -DdeadlockDetection=on.

6. Run it in reachability testing (RT) mode with symmetry reduction. The Producer threads have identical
   behavior, i.e., each Producer executes call to (re)enter the monitor in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Producer threads by ignoring the diferent orders that Producer or Consumer threads 
   execute the same operation. For example, each Producer thread begins by calling deposit(). Normal RT 
   will exercise the 6 possible orders in which the three Producer threads can enter deposit(). With 
   symmetry reduction, RT ignores the races that occur between Producer threads entering deposit(). This 
   causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program BoundedBufferMonitorSU
   the contents of symmetry.txt is:

   2 3 4
   5 6 7

   The first group - 2 3 4 - represents the three Producer threads. The second group - 5 6 7 - represents
   the three Consumer threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt. (Make sure you use the
   identifiers generated by rt mode, not trace mode, since the identifiers may be different. In rt
   mode, the synchronization objects (e.g., monitors) receive identifiers too.)

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferMonitorSU
   Unix:      java -Dmode=rt -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferMonitorSU

   The output should look something like:

start:Mon Jan 23 10:37:08 GMT-05:00 2006
1/1

Reachability Testing completed.
  Executions:20 / Sequences Collected:20
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000
