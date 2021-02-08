Program ReadersWritersSelect is a solution to the readers
and writers problem using a selective wait with 3 readers and 2 writers.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  ReadersWritersSelect.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  ReadersWritersSelect.java  

2. Run it in trace mode:
   Windows:   java -Dmode=trace -Dstrategy=thread -classpath .;../ModernMultithreading.jar  ReadersWritersSelect
   Unix:      java -Dmode=trace -Dstrategy=thread -classpath .:../ModernMultithreading.jar  ReadersWritersSelect

This will produce 4 files:
- ThreadID.txt: Thread IDs that were generated 
- ChannelID.txt : The channel names and IDs that were used
- channel-replay.txt: simple SR-sequence collected during execution
- channel-test.txt: SR-sequence collected during execution 

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of send operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -Dstrategy=thread -classpath .;../ModernMultithreading.jar  ReadersWritersSelect
   Unix:      java -Dmode=replay -Dstrategy=thread -classpath .:../ModernMultithreading.jar  ReadersWritersSelect

When the sequence in Y.txt has been replayed, you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -Dstrategy=thread -classpath .;../ModernMultithreading.jar  ReadersWritersSelect
   Unix:      java -Dmode=test -Dstrategy=thread -classpath .:../ModernMultithreading.jar  ReadersWritersSelect

This will determine whether the SR-sequence in channel-test.txt is feasible; you will 
see: Sequence Completed, indicating that the sequence is feasible. Note, since the
sequence in channel-test.txt was collected when we traced the execution, so it is 
definitely feasible. You can put SR-sequences of your own in channel-test.txt and
determine whether they are feasible.

5. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -Dstrategy=thread -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersSelect
    Unix:      java -Dmode=rt -Dstrategy=thread -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersSelect

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is: 

start:Tue Dec 06 08:00:53 GMT-05:00 2005
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
725/725
750/750

Reachability Testing completed.
  Executions:768 / Sequences Collected:768
  Elapsed time in minutes: 0.3333333333333333
  Elapsed time in seconds: 20.0
  Elapsed time in milliseconds: 20000


After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

6. Run it in reachability testing (RT) mode with symmetry reduction. The Reader threads have identical
   behavior, i.e., each Reader executes calls to (re)enter the monitor in the same order, 
   independent of any inputs, and independent of the identity of the thread.  We can take advantage of 
   the symmetry of the Reader threads by ignoring the diferent orders that Reader or Writer threads 
   execute the same operation. For example, each Reader thread begins by calling startRead.send(). Normal 
   RT will exercise the 6 possible orders in which these sends can be received. With symmetry reduction, 
   RT ignores the races that occur between these sends. This causes fewer sequences to be exercised.

   To use the symmetry reduction, first create a file called symmetry.txt. Each line of this file specifies
   the thread identifiers of one group of symmetric threads. For example, for program 
   ReadersWritersMonitorSelect the contents of symmetry.txt is:

   2 3 4
   5 6

   The first group - 2 3 4 - represents the three Reader threads. The second group - 5 6 - represents
   the two Write threads. Each thread in the program should be listed in one and only one group.
   To see the thread identifiers, you can first run the program for a few seconds in rt mode without 
   symmetry reduction and examine the identifiers in file ThreadID.txt.

   Run reachability testing with symmetry reduction using:

   Windows:   java -Dmode=rt -Dstrategy=thread -DsymmetryReduction=on -classpath .;../ModernMultithreading.jar  RTDriver ReadersWritersSelect
   Unix:      java -Dmode=rt -Dstrategy=thread -DsymmetryReduction=on -classpath .:../ModernMultithreading.jar  RTDriver ReadersWritersSelect

   The output should look something like:

start:Mon Jan 23 11:19:56 GMT-05:00 2006
1/1
25/25

Reachability Testing completed.
  Executions:28 / Sequences Collected:28
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000
