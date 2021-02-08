Program DistributedSpanningTree is an implementation of the 
distributed spanning tree problem with 3 nodes.
It is adapted from:

Vijay K. Garg, Concurrent and Distributed Computing in Java, 
Wiley, 2005.

This program has three nodes:

       0
      / \
     1 -- 2

So the possible spanning trees are:

       0
      / \
     1   2

       0
      / 
     1 -- 2

       0
        \
     1 -- 2

The first spanning tree would be represented as follows:

parent of 0 is 0, since 0 is the root and the parent of the root node is always the node itself.
parent of 1 is 0, since 1 is connected to 0
parent of 2 is 0, since 2 is connected to 0

1. Compile the program and the utility files:
   Windows:   javac -classpath .;../ModernMultithreading.jar DistributedSpanningTree.java
              javac -classpath .;../ModernMultithreading.jar childrenArray.java
              javac -classpath .;../ModernMultithreading.jar neighborMatrix.java
              javac -classpath .;../ModernMultithreading.jar DST.java
   Unix:      javac -classpath .:../ModernMultithreading.jar DistributedSpanningTree.java
              javac -classpath .:../ModernMultithreading.jar childrenArray.java
              javac -classpath .:../ModernMultithreading.jar neighborMatrix.java
              javac -classpath .:../ModernMultithreading.jar DST.jav

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar DST
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar DST

This will produce 9 files:
- ThreadID.txt: Thread IDs that were generated 
- ChannelID.txt : The channel names and IDs that were used
- channel-replay.txt: simple SR-sequence collected during execution
- channel-test.txt: SR-sequence collected during execution 
- conditionVarID.txt: The IDs assigned to condition variables

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar DST
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar DST

When the sequence in channel-replay.txt has been replayed; you will see: Sequence Completed

4. Run it in test mode:
   Windows:   java -Dmode=test -classpath .;../ModernMultithreading.jar DST
   Unix:      java -Dmode=test -classpath .:../ModernMultithreading.jar DST

This will determine whether the SR-sequence in channel-test.txt is feasible; 
you will see: Sequence Completed, indicating that the sequence is feasible. 
Note, since the sequence in channel-test.txt was collected when we traced the execution, and
assuming you havn't changed the program, the sequence is definitely feasible. 
You can put sequences of your own in channel-test.txt and determine whether they are feasible.

5. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver DST
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver DST

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Wed Jun 21 15:56:13 GMT-05:00 2006
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

6. Check the sequences that are exercised during RT.

When node i chooses parent j, it executes:

   exerciseEvent("parent i j");

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
 
   Windows:   javac -classpath .;../ModernMultithreading.jar MyChecker.java
   Unix:      javac -classpath .:../ModernMultithreading.jar MyChecker.java

Perform RT using:

    Windows:   java -Dmode=rt -DcheckTrace=on -classpath .;../ModernMultithreading.jar RTDriver DST
    Unix:      java -Dmode=rt -DcheckTrace=on -classpath .:../ModernMultithreading.jar RTDriver DST

Our check() method for DistributedSpanningTree checks that the parents chosen by the nodes
result in a spanning tree. We also run RT with deadlock detection (-DdeadlockDetection=on).