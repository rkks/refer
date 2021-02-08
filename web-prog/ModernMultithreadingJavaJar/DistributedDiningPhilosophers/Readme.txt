Program DistributedDiningPhilosophers is an implementation of the 
distributed dining philosophers problem with 3 philosophers.
It is adapted from:

Vijay K. Garg, Concurrent and Distributed Computing in Java, 
Wiley, 2004.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  DistributedDiningPhilosophers.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  DistributedDiningPhilosophers

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  DistributedDiningPhilosophers
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  DistributedDiningPhilosophers

This will produce 9 files:
- ThreadID.txt: Thread IDs that were generated 
- monitorID.txt: The monitor name that was used
- monitor-replay.txt: simple M-sequence collected during execution
- monitor-test.txt: M-sequence collected during execution 
- monitor-spectest.txt: communication-sequence collected during execution
- ChannelID.txt : The channel names and IDs that were used
- channel-replay.txt: simple SR-sequence collected during execution
- channel-test.txt: SR-sequence collected during execution 
- conditionVarID.txt: The IDs assigned to condition variables

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  DistributedDiningPhilosophers
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  DistributedDiningPhilosophers

When the sequences have been replayed, you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar  DistributedDiningPhilosophers
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar  DistributedDiningPhilosophers

This will determine whether the M-sequence in monitor-test.txt is feasible, and the
SR-sequence in channel-test.txt is feasible; you will see: Sequence Completed, 
indicating that the sequence is feasible. Note, since the sequences in monitor-test.txt 
and channel-test.txt were collected when we traced the execution, they are 
definitely feasible. You can put sequences of your own in monitor-test.txt and
channel-test.txt and determine whether they are feasible.

5. Run t-way reachability testing (RT). Normal RT exercises a very large number
   of sequences. Running 1-way RT exercises considerably fewer sequences.

    Windows:   java -Dmode=rt -Dtway=on -Dt=1 -classpath .;../ModernMultithreading.jar  RTDriver DistributedDiningPhilosophers
    Unix:      java -Dmode=rt -Dtway=on -Dt=1 -classpath .:../ModernMultithreading.jar  RTDriver DistributedDiningPhilosophers

Property -Dt specifies the value for t, which is usually a small number such as 1 or 2.

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Wed Dec 07 11:01:01 GMT-05:00 2005
1/1
25/25
50/50
75/75
100/100
...

3600/3600
3625/3625

Reachability Testing completed.
  Executions:3644 / Sequences Collected:3644
  Elapsed time in minutes: 4.167183333333333
  Elapsed time in seconds: 250.031
  Elapsed time in milliseconds: 250031

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The number of sequenes that are exercised may be different each time you run t-way reachability testing.
The actual elapsed time may be 4 seconds more or less than shown.

6. Check the sequences that are exercised during 1-way RT.

When each process enters its critical section, it executes:

   exerciseEvent("enter");

These user-level events are collected and at the end of each execution this sequence of 
events can be passed to a user-supplied check() routine that checks the exercised
sequence for correctness.

To check sequences, write the following class:

  public class MyChecker implements Checker {
    public boolean check (ArrayList seq) {
       /* your check routine here */
    }
  }

and supply the implementation of method check(). Method check() will receive an ArrayList of 
AppEvent, where an AppEvent is:

public class AppEvent {
  private int tid; // thread id
  private String label; // event label
  private vectorTimeStamp stamp; // event timestamp
  public AppEvent (int tid, String label, vectorTimeStamp stamp) {
    this.tid = tid;    this.label = label;    this.stamp = stamp;
  }
  public int getThreadID () {return tid;}
  public String getLabel () {return label;}
  public vectorTimeStamp getTimeStamp () {return stamp;}
  public boolean happenBefore (AppEvent another) {return stamp.lessThan (another.getTimeStamp ());}
  public boolean isConcurrent (AppEvent another) {
    return !happenBefore(another) && !another.happenBefore(this);
  }
  public String toString () {
    StringBuffer rval = new StringBuffer ();
    rval.append("AppEvent [");   rval.append(tid);   rval.append(", ");   rval.append(label + ", ");
    rval.append(stamp);   rval.append("]");  return rval.toString ();
  }
}

AppEvents are created by calls to exerciseEvent("label"). Each event contains the label the ThreadID
of the calling thread, and a vector timestamp value for the event.

Compile MyChecker.java:
 
   Windows:   javac -classpath .;../ModernMultithreading.jar  MyChecker.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  MyChecker.java

Perform RT using:

    Windows:   java -Dmode=rt -DcheckTrace=on -DtWay=on -Dt=1 -classpath .;../ModernMultithreading.jar  RTDriver DistributedDiningPhilosophers
    Unix:      java -Dmode=rt -DcheckTrace=on -DtWay=on -Dt=1 -classpath .:../ModernMultithreading.jar  RTDriver DistributedDiningPhilosophers

Our check() method for DistributedDiningPhilosophers checks that no neighboring philosophers
eat concurrently and that all philosophers eventually eat. We also run RT with deadlock 
detection (-DdeadlockDetection=on).
