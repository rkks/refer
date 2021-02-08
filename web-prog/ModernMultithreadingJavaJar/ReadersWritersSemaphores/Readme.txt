Program ReadersWritersSemaphores is a semaphore solution to the readers
and writers problem with 3 readers and 2 writers.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  ReadersWritersSemaphores.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  ReadersWritersSemaphores.java  

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  ReadersWritersSemaphores
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  ReadersWritersSemaphores

This will produce 3 files:
- ThreadID.txt: Thread IDs that were generated 
- semaphoreID.txt: semaphore names that were generated
- semaphore-replay.txt: simple PV-sequence that can be used to replay the program

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  ReadersWritersSemaphores
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  ReadersWritersSemaphores

To see what happens when there is a deadlock, change the mutex semaphore so that it is
initialized to 0:

   mutex = new countingSemaphore(0,"mutex");

Now all the reader and writer threads will block themselves. Run the program in trace 
mode with deadlockDetection on and you will see:

  Monitoring for Deadlock.
  Deadlock detected:
  - Reader3 blocked on P operation of mutex
  - Reader2 blocked on P operation of mutex
  - Writer1 blocked on P operation of mutex
  - Writer2 blocked on P operation of mutex
  - Reader1 blocked on P operation of mutex

  Execution trace:
  Thread Reader3 blocking on mutex.P()
  Thread Writer2 blocking on mutex.P()
  Thread Reader2 blocking on mutex.P()
  Thread Writer1 blocking on mutex.P()
  Thread Reader1 blocking on mutex.P()

*** Make sure you change the initialization code back to the way it was:
      mutex = new countingSemaphore(1,"mutex");
    and recompile the program. 

    Run it again in TRACE mode to collect a non-deadlock sequence so we can replay it below.
***

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of P and V operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  ReadersWritersSemaphores
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  ReadersWritersSemaphores

When the sequence in semaphore-replay.txt has been replayed, you will see: Sequence Completed

4. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersSemaphores
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersSemaphores

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Mon Nov 28 20:24:19 GMT-05:00 2005
1/1
25/25
50/50
75/75
100/100
...
21600/21600
21625/21625
21650/21650
21675/21675
21700/21700
21725/21725

Reachability Testing completed.
  Executions:21744 / Sequences Collected:21744
  Elapsed time in minutes: 16.460933333333333
  Elapsed time in seconds: 987.656
  Elapsed time in milliseconds: 987656


After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

You can also run RT mode with -DdeadlockDetection=on.

5. Run it in reachability testing (RT) mode with symmetry reduction. The Reader threads have identical
   behavior, i.e., each Readerexecutes P and V operations on the same semaphores, in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Reader threads by ignoring the diferent orders that Reader or Writer threads 
   execute the same operation. For example, each Reader thread begins by executing mutex.P(). Normal RT
   will exercise the 6 possible orders in which the three Reader threads can execute mutex.P(). With 
   symmetry reduction, RT ignores the races that occur between Reader threads executing mutex.P(). This 
   causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program 
   ReadersWritersSemaphores the contents of symmetry.txt is:

   4 5 6
   7 8

   The first group - 4 5 6 - represents the three Reader threads. The second group - 7 8 - represents
   the two Writer threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt. (Make sure you use the
   identifiers generated by rt mode, not trace mode, since the identifiers may be different. In rt
   mode, the synchronization objects (e.g., semaphores) receive identifiers too.)

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersSemaphores
   Unix:      java -Dmode=rt -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersSemaphores

   The output should look something like:

start:Mon Jan 23 10:52:46 GMT-05:00 2006
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
  Elapsed time in minutes: 0.3333333333333333
  Elapsed time in seconds: 20.0
  Elapsed time in milliseconds: 20000

