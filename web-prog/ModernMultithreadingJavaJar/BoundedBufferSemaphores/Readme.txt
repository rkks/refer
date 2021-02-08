Program BoundedBufferSemaphores is a semaphore solution to the bounded
buffer problem with 3 producer, 3 consumers, and a 2-slot buffer.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  BoundedBufferSemaphores.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  BoundedBufferSemaphores.java  

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  BoundedBufferSemaphores
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  BoundedBufferSemaphores

This will produce 3 files:
- ThreadID.txt: Thread IDs that were generated 
- semaphoreID.txt: semaphore names that were generated
- semaphore-replay.txt: simple PV-sequence that can be used to replay the program

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  BoundedBufferSemaphores
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  BoundedBufferSemaphores

To see what happens when there is a deadlock, change the mutexD and mutexW semaphores so that 
they are both initialized to 0:

   mutexD = new countingSemaphore(0,"mutexD");
   mutexW = new countingSemaphore(0,"mutexW");

Now all the producer and consumer threads will block themselves. Run the program in trace 
mode with deadlockDetection on and you will see:

Monitoring for Deadlock.
Deadlock detected:
- Consumer2 blocked on P operation of mutexW
- Producer2 blocked on P operation of mutexD
- Consumer3 blocked on P operation of mutexW
- Producer1 blocked on P operation of mutexD
- Producer3 blocked on P operation of mutexD
- Consumer1 blocked on P operation of mutexW

Execution trace:
Thread Producer1 blocking on mutexD.P()
Thread Producer2 blocking on mutexD.P()
Thread Consumer1 blocking on mutexW.P()
Thread Consumer2 blocking on mutexW.P()
Thread Consumer3 blocking on mutexW.P()
Thread Producer3 blocking on mutexD.P()

*** Make sure you change the initialization code back to the way it was:
      mutexD = new countingSemaphore(1,"mutexD");
      mutexW = new countingSemaphore(1,"mutexW");
    and recompile the program.  Run it again in TRACE mode to collect a non-deadlock 
    sequence so we can replay it below. 
***

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of P and V operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  BoundedBufferSemaphores
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  BoundedBufferSemaphores

When the sequence in semaphore-replay.txt has been replayed, you will see: Sequence Completed

4. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Mon Nov 14 15:24:35 GMT-05:00 2005
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

Reachability Testing completed.
  Executions:324 / Sequences Collected:324
  Elapsed time in minutes: 0.4166666666666667
  Elapsed time in seconds: 25.0
  Elapsed time in milliseconds: 25000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

You can also run RT mode with -DdeadlockDetection=on.

5. Run it in reachability testing (RT) mode with the PVReduction, which (safely) ignores
   some races between P and V operations, e.g., a race between two V operations.
   This optimization will often cause fewer sequences to be exercised:

   Windows:   java -Dmode=rt -DPVReduction=on -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores
   Unix:      java -Dmode=rt -DPVReduction=on -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores

   The output should look something like:

start:Mon Nov 14 15:30:23 GMT-05:00 2005
1/1
25/25
50/50

Reachability Testing completed.
  Executions:56 / Sequences Collected:56
  Elapsed time in minutes: 0.11666666666666667
  Elapsed time in seconds: 7.0
  Elapsed time in milliseconds: 7000

The number of sequences exercised may vary, depending on the races that actually occur when the
program is executed.

The actual elapsed time may be up to 4 seconds more or less than shown.

6. Run it in reachability testing (RT) mode with symmetry reduction. The Producer threads have identical
   behavior, i.e., each Producer executes P and V operations on the same semaphores, in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Producer threads by ignoring the different orders that Producer  or Consumer threads 
   execute the same operation. For example, each Producer thread begins by executing mutexD.P(). Normal RT
   will exercise the 6 possible orders in which the three Producer threads can execute mutexD.P(). With 
   symmetry reduction, RT ignores the races that occur between Producer threads executing mutexD.P(). This 
   causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program BoundedBufferSemaphores
   the contents of symmetry.txt is:

   5 6 7 
   8 9 10

   The first group - 5 6 7 - represents the three Producer threads. The second group - 8 9 10 - represents
   the three Consumer threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt. (Make sure you use the
   identifiers generated by rt mode, not trace mode, since the identifiers may be different. In rt
   mode, the synchronization objects (e.g., semaphores) receive identifiers too.)

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores
   Unix:      java -Dmode=rt -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores

   The output should look something like:

start:Mon Jan 23 10:15:31 GMT-05:00 2006
1/1

Reachability Testing completed.
  Executions:9 / Sequences Collected:9
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000

7. Run it in reachability testing (RT) mode with symmetry reduction and the PVReduction:

   Windows:   java -Dmode=rt -DPVReduction=on -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores
   Unix:      java -Dmode=rt -DPVReduction=on -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSemaphores

The output should look something like:

start:Mon Jan 23 10:17:49 GMT-05:00 2006
1/1

Reachability Testing completed.
  Executions:1 / Sequences Collected:1
  Elapsed time in minutes: -0.06666666666666667
  Elapsed time in seconds: -4.0
  Elapsed time in milliseconds: -4000

You may also exercise 3 sequences. (The number of sequences exercised varies, depending on the P/V races that 
actually occur when the program is executed.)
