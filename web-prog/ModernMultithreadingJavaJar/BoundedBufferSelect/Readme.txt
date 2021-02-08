Program BoundedBufferSelect is a selective-wait based solution to the 
bounded buffer problem with 3 producer, 3 consumers, and a 2-slot buffer.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  BoundedBufferSelect.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar  BoundedBufferSelect.java

2. Run it in trace mode - you must specify -Dstrategy=thread when the program contains
   a selective wait:
   Windows:   java -Dmode=trace -Dstrategy=thread -classpath .;../ModernMultithreading.jar  BoundedBufferSelect
   Unix:      java -Dmode=trace -Dstrategy=thread -classpath .:../ModernMultithreading.jar  BoundedBufferSelect

This will produce 4 files:
- ThreadID.txt: Thread IDs that were generated 
- ChannelID.txt : The channel names and IDs that were used
- channel-replay.txt: simple SR-sequence collected during execution
- channel-test.txt: SR-sequence collected during execution 

Note that in BoundedBufferSelect.java, each channel has an owner, which is the thread that
executes receive events on the channel:

  deposit.setOwner(b);          // buffer thread b executes deposit.receive();
  withdraw.setOwner(b);
  replyPortC1.setOwner(c1);     // consumer thread c1 executes replyPortC1.receive();
  replyPortC2.setOwner(c2);
  replyPortC3.setOwner(c3);

This is necessary in order to perform replay and testing of channel-based programs.

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of send operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -Dstrategy=thread -classpath .;../ModernMultithreading.jar  BoundedBufferSelect
   Unix:      java -Dmode=replay -Dstrategy=thread -classpath .:../ModernMultithreading.jar  BoundedBufferSelect

When the sequence in channel-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -Dstrategy=thread -classpath .;../ModernMultithreading.jar  BoundedBufferSelect
   Unix:      java -Dmode=test -Dstrategy=thread -classpath .:../ModernMultithreading.jar  BoundedBufferSelect

This will determine whether the SR-sequence in channel-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in channel-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put SR-sequences of your own in channel-test.txt and
determine whether they are feasible.

5. Run it in reachability testing (RT) mode. You must specify -Dstrategy=thread
   since the program contains a selective wait.

    Windows:   java -Dmode=rt -Dstrategy=thread -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSelect
    Unix:      java -Dmode=rt -Dstrategy=thread -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSelect

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Sun Nov 20 09:35:44 GMT-05:00 2005
1/1
25/25
50/50
75/75
100/100
125/125

Reachability Testing completed.
  Executions:144 / Sequences Collected:144
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

6. Run it in reachability testing (RT) mode with symmetry reduction. The Producer threads have identical
   behavior, i.e., each Producer executes call to (re)enter the monitor in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Producer threads by ignoring the diferent orders that Producer or Consumer threads 
   execute the same operation. For example, each Producer thread begins by calling deposit.send(). Normal 
   RT will exercise the 6 possible orders in which the calls to deposit.send() can be received. With symmetry
   reduction, RT ignores the races that occur between Producer threads. This causes fewer sequences to 
   be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program 
   BoundedBufferMonitorSelect the contents of symmetry.txt is:

   2 3 4
   5 6 7

   The first group - 2 3 4 - represents the three Producer threads. The second group - 5 6 7 - represents
   the three Consumer threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt. (Make sure you use the
   identifiers generated by rt mode, not trace mode, since the identifiers may be different. In rt
   mode, the synchronization objects (e.g., ports) receive identifiers too.)

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -Dstrategy=thread -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver BoundedBufferSelect 
   Unix:      java -Dmode=rt -Dstrategy=thread -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver BoundedBufferSelect

   The output should look something like:

start:Mon Jan 23 10:45:28 GMT-05:00 2006
1/1

Reachability Testing completed.
  Executions:4 / Sequences Collected:4
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000


