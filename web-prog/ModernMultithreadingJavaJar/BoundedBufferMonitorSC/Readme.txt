Program boundedBufferMonitorSU is a semaphore solution to the bounded
buffer problem with 3 producer, 3 consumers, and a 2-slot buffer.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC.java

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC 

This will produce 6 files:
- ThreadID.txt: Thread IDs that were generated 
- conditionVarID.txt : The conditionVariables names that were used
- monitorID.txt: The monitor name that was used
- monitor-replay.txt: simple M-sequence collected during execution
- monitor-test.txt: M-sequence collected during execution 
- monitor-spectest.txt: communication-sequence collected during execution

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC 
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC

To see what happens when there is a deadlock, you can change the while-loop in deposit()
to:

   while (fullSlots < capacity )  {
     notFull.waitC();
   }

so that all the producer and consumer threads will block themselves. Run the program in trace 
mode with deadlockDetection on and you will see something like:

  Monitoring for Deadlock.
  Deadlock detected:
  - Producer1 blocked on waitC() in method Buffer:deposit
  - Consumer1 blocked on waitC() in method Buffer:withdraw
  - Producer2 blocked on waitC() in method Buffer:deposit
  - Producer3 blocked on waitC() in method Buffer:deposit
  - Consumer3 blocked on waitC() in method Buffer:withdraw
  - Consumer2 blocked on waitC() in method Buffer:withdraw

  Execution trace:
  Thread Consumer3 entered monitor method withdraw
  Thread Consumer3 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Consumer1 entered monitor method withdraw
  Thread Consumer1 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Consumer2 entered monitor method withdraw
  Thread Consumer2 blocked on notEmpty.waitC() in Buffer:withdraw
  Thread Producer3 entered monitor method deposit
  Thread Producer3 blocked on notFull.waitC() in Buffer:deposit
  Thread Producer2 entered monitor method deposit
  Thread Producer2 blocked on notFull.waitC() in Buffer:deposit
  Thread Producer1 entered monitor method deposit
  Thread Producer1 blocked on notFull.waitC() in Buffer:deposit

*** Make sure you change the while-loop back to the way it was and recompile the program.

    Run it again in TRACE mode to collect a non-deadlock sequence so we can replay it below. 
***

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at monitor entry.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC 

When the sequence in semaphore-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC

This will determine whether the M-sequence in monitor-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in monitor-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put M-sequences of your own in monitor-test.txt and
determine whether they are feasible.

5. Run it in spectest mode:
   Windows:   java -Dmode=spectest -classpath .;../ModernMultithreading.jar BoundedBufferMonitorSC
   Unix:      java -Dmode=spectest -classpath .:../ModernMultithreading.jar BoundedBufferMonitorSC 

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

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver BoundedBufferMonitorSC
    Unix:      java -Dmode=rt -classpath .:./ModernMultithreading.jar RTDriver BoundedBufferMonitorSC

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

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
...
12000/12000
12025/12025
12050/12050
12075/12075

Reachability Testing completed.
  Executions:12096 / Sequences Collected:12096
  Elapsed time in minutes: 3.4
  Elapsed time in seconds: 204.0
  Elapsed time in milliseconds: 204000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

You can also run RT mode with -DdeadlockDetection=on.

7. Run it in reachability testing (RT) mode with symmetry reduction. The Producer threads have identical
   behavior, i.e., each Producer executes call to (re)enter the monitor in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Producer threads by ignoring the diferent orders that Producer or Consumer threads 
   execute the same operation. For example, each Producer thread begins by calling deposit(). Normal RT 
   will exercise the 6 possible orders in which the three Producer threads can enter deposit(). With 
   symmetry reduction, RT ignores the races that occur between Producer threads entering deposit(). This 
   causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program BoundedBufferMonitorSC
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

   Windows:   java -Dmode=rt -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar RTDriver BoundedBufferMonitorSC
   Unix:      java -Dmode=rt -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar RTDriver BoundedBufferMonitorSC

   The output should look something like:

start:Mon Jan 23 10:32:12 GMT-05:00 2006
1/1
25/25
50/50
75/75
100/100
125/125

Reachability Testing completed.
  Executions:132 / Sequences Collected:132
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000

