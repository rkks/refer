Program ReadersWritersMonitorSU is an SU monitor to the readers
and writers problem with 3 readers and 2 writers.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU.java

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU

This will produce 6 files:
- ThreadID.txt: Thread IDs that were generated 
- conditionVarID.txt : The conditionVariables names that were used
- monitorID.txt: The monitor name that was used
- monitor-replay.txt: simple M-sequence collected during execution
- monitor-test.txt: M-sequence collected during execution 
- monitor-spectest.txt: communication-sequence collected during execution

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU

To see what happens when there is a deadlock, you can change the if-statements in startRead()
and startWrite() to:

   if (true) ...

so that all the reader and writer threads will block themselves. Run the program in trace 
mode with deadlockDetection on and you will see:

  Monitoring for Deadlock.
  Deadlock detected:
  - Reader1 blocked on waitC() in method RWController:startRead
  - Writer2 blocked on waitC() in method RWController:startWrite
  - Writer1 blocked on waitC() in method RWController:startWrite
  - Reader3 blocked on waitC() in method RWController:startRead
  - Reader2 blocked on waitC() in method RWController:startRead

  Execution trace:
  Thread Reader3 entered monitor method startRead
  Thread Reader3 blocked on readerQ.waitC() in RWController:startRead
  Thread Reader1 entered monitor method startRead
  Thread Reader1 blocked on readerQ.waitC() in RWController:startRead
  Thread Writer2 entered monitor method startWrite
  Thread Writer2 blocked on writerQ.waitC() in RWController:startWrite
  Thread Reader2 entered monitor method startRead
  Thread Reader2 blocked on readerQ.waitC() in RWController:startRead
  Thread Writer1 entered monitor method startWrite
  Thread Writer1 blocked on writerQ.waitC() in RWController:startWrite


*** Make sure you change the if-statement back to the way it was and recompile the program.

    Run it again in TRACE mode to collect a non-deadlock sequence so we can replay it below. 
***

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at monitor entry.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU

When the sequence in semaphore-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU

This will determine whether the M-sequence in monitor-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in monitor-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put M-sequences of your own in monitor-test.txt and
determine whether they are feasible.

5. Run it in spectest mode:
   Windows:   java -Dmode=spectest -classpath .;../ModernMultithreading.jar  ReadersWritersMonitorSU
   Unix:      java -Dmode=spectest -classpath .:../ModernMultithreading.jar  ReadersWritersMonitorSU

This will determine whether the communication-sequence in monitor-spectest.txt is feasible; 
you will see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in monitor-spectest.txt was collected when we traced the execution, so it is 
definitely feasible. You can put communication-sequences of your own in monitor-spectest.txt 
and determine whether they are feasible.

For example, change the first event in monitor-spectest.txt to be:

  (1,endRead)

and run it in spectest mode again. Since no sequence can start with a withdrawl by a consumer,
this sequence is infeasible. You will see: 
   Infeasible Sequence at event 1, unexpected event name request
   Expected event 1 was: (1,endRead)

6. Run it in reachability testing (RT) mode.
:
    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersMonitorSU
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersMonitorSU

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Mon Nov 28 22:25:00 GMT-05:00 2005
1/1
25/25
50/50
75/75
100/100
...
13200/13200
13225/13225
13250/13250
13275/13275
13300/13300

Reachability Testing completed.
  Executions:13320 / Sequences Collected:13320
  Elapsed time in minutes: 14.066666666666666
  Elapsed time in seconds: 844.0
  Elapsed time in milliseconds: 844000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

You can also run RT mode with -DdeadlockDetection=on.

7. Run it in reachability testing (RT) mode with symmetry reduction. The Reader threads have identical
   behavior, i.e., each Reader executes calls to (re)enter the monitor in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Reader threads by ignoring the diferent orders that Reader or Writer threads 
   execute the same operation. For example, each Reader thread begins by calling startRead(). Normal RT 
   will exercise the 6 possible orders in which the three Reader threads can enter startRead(). With 
   symmetry reduction, RT ignores the races that occur between Readerthreads entering startRead(). This 
   causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program ReadersWritersMonitorSU
   the contents of symmetry.txt is:

   2 3 4
   5 6

   The first group - 2 3 4 - represents the three Reader threads. The second group - 5 6 - represents
   the two Write threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt. (Make sure you use the
   identifiers generated by rt mode, not trace mode, since the identifiers may be different. In rt
   mode, the synchronization objects (e.g., monitors) receive identifiers too.)

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersMonitorSU
   Unix:      java -Dmode=rt -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersMonitorSU

   The output should look something like:

start:Mon Jan 23 11:10:37 GMT-05:00 2006
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

Reachability Testing completed.
  Executions:420 / Sequences Collected:420
  Elapsed time in minutes: 0.6
  Elapsed time in seconds: 36.0
  Elapsed time in milliseconds: 36000


