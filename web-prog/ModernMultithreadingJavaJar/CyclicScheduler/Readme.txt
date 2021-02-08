Program Cyclic Scheduler is an implementation of Milner's cyclic scheduler.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  CyclicScheduler.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar  CyclicScheduler.java 

2. Run it in trace mode - you must specify -Dstrategy=thread when the program contains
   a selective wait:
   Windows:   java -Dmode=trace -Dstrategy=thread -classpath .;../ModernMultithreading.jar  CyclicScheduler
   Unix:      java -Dmode=trace -Dstrategy=thread -classpath .:../ModernMultithreading.jar  CyclicScheduler

This will produce 4 files:
- ThreadID.txt: Thread IDs that were generated 
- ChannelID.txt : The channel names and IDs that were used
- channel-replay.txt: simple SR-sequence collected during execution
- channel-test.txt: SR-sequence collected during execution 

Note that in CyclicScheduler each channel has an owner, which is the thread that
executes receive events on the channel:

   for (int i = 0; i<size; i++) {
      clients[i] = new Client(next,finish,start,i);
      start[i].setOwner(clients[i]);
      schedulers[i] = new Scheduler(next,finish,start,i);
      next[i].setOwner(schedulers[i]);
      finish[i].setOwner(schedulers[i]);
      clients[i].start(); schedulers[i].start();
   }

This is necessary in order to perform replay and testing of channel-based programs.

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of send operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -Dstrategy=thread -classpath .;../ModernMultithreading.jar  CyclicScheduler
   Unix:      java -Dmode=replay -Dstrategy=thread -classpath .:../ModernMultithreading.jar  CyclicScheduler

When the sequence in channel-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -Dstrategy=thread -classpath .;../ModernMultithreading.jar  CyclicScheduler
   Unix:      java -Dmode=test -Dstrategy=thread -classpath .:../ModernMultithreading.jar  CyclicScheduler

This will determine whether the SR-sequence in channel-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in channel-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put SR-sequences of your own in channel-test.txt and
determine whether they are feasible.

5. Run it in reachability testing (RT) mode. You must specify -Dstrategy=thread
   since the program contains a selective wait.

    Windows:   java -Dmode=rt -Dstrategy=thread -classpath .;../ModernMultithreading.jar  RTDriver CyclicScheduler
    Unix:      java -Dmode=rt -Dstrategy=thread -classpath .:../ModernMultithreading.jar  RTDriver CyclicScheduler

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:
start:Wed Dec 21 09:57:00 GMT-05:00 2005
1/1

Reachability Testing completed.
  Executions:8 / Sequences Collected:8
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000


After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

