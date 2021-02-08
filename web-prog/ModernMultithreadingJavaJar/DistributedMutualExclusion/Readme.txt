Program DistributedMutualExclusion is an implementation of the
distributed mutual exclusion algorithm in:

G. Ricart and A. K. Agrawala, An optimal algorithm for mutual 
exclusion in computer networks, Communications of the ACM, 24, 
1 (January), 1981, pp. 9-17.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar DistributedMutualExclusion.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar DistributedMutualExclusion.java

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar DistributedMutualExclusion
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar DistributedMutualExclusion

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

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of send operations.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar DistributedMutualExclusion
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar DistributedMutualExclusion

When the sequence in semaphore-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar DistributedMutualExclusion
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar DistributedMutualExclusion

This will determine whether the M-sequence in monitor-test.txt is feasible, and the
SR-sequence in channel-test.txt is feasible; you will see: Sequence Completed, 
indicating that the sequence is feasible. Note, since the sequences in monitor-test.txt 
and channel-test.txt were collected when we traced the execution, they are 
definitely feasible. You can put sequences of your own in monitor-test.txt and
channel-test.txt and determine whether they are feasible.


5. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver DistributedMutualExclusion
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver DistributedMutualExclusion

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
3900/3900
3925/3925
3950/3950
3975/3975
4000/4000
4025/4025

Reachability Testing completed.
  Executions:4032 / Sequences Collected:4032
  Elapsed time in minutes: 2.8666666666666667
  Elapsed time in seconds: 172.0
  Elapsed time in milliseconds: 172000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

6. Check the sequences that are exercised during RT.

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
  private Object obj;

  public AppEvent (int tid, String label, Object obj, vectorTimeStamp stamp) {
    this.tid = tid;   this.label = label;   
    this.stamp = stamp;   this.obj = obj;
  }

  public int getThreadID () { return tid; }
  public String getLabel () { return label;}
  public vectorTimeStamp getTimeStamp () { return stamp; }
  public Object getObject () { return obj;}

  public boolean happenBefore (AppEvent another) {
    return stamp.lessThan (another.getTimeStamp ());
  }

  public boolean isConcurrent (AppEvent another) {
    return !happenBefore(another) && !another.happenBefore(this);
  }

  public String toString () {
    StringBuffer rval = new StringBuffer ();
    rval.append("AppEvent [");    rval.append(tid);    rval.append(", ");    
    rval.append(label + ", ");    rval.append(obj + ", ");    rval.append(stamp);  
    rval.append("]");
    return rval.toString ();
  }
}


AppEvents are created by calls to exerciseEvent("label"). Each event contains the 
label the ThreadID of the calling thread, and a vector timestamp value for the event.

Compile MyChecker.java:
 
   Windows:   javac -classpath .;../ModernMultithreading.jar MyChecker.java
   Unix:      javac -classpath .:../ModernMultithreading.jar MyChecker.java

Perform RT using:

    Windows:   java -Dmode=rt -DcheckTrace=on -classpath .;../ModernMultithreading.jar RTDriver DistributedMutualExclusion
    Unix:      java -Dmode=rt -DcheckTrace=on -classpath .:../ModernMultithreading.jar RTDriver DistributedMutualExclusion

Our check() method for DistributedMutualExclusion checks that none of the "enter" events are
executed concurrently and that all processes eventually enter their critical sections.
You can also run RT with deadlock detection (-DdeadlockDetection=on).
