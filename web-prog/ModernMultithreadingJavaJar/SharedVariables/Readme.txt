Program SharedVariables illustrates the use of the shared 
variable classes to detect data races during reachability testing.

Program SharedVariable uses special shared object classes that monitor
Read and Write operations. The shared object classes are just wrappers
for primitive types int, booleab, double, and long:

   sharedInteger s = new sharedInteger(0,"s");
   sharedBoolean b = new sharedBoolean(true,"b");
   sharedDouble d = new sharedDouble(1.0,"d");
   sharedLong l = new sharedLong(1,"l")

Each shared object class supplies Read and Write operations for the shared
objects. For example:

   System.out.println(s.Read());
   s.Write(s.Read() + 1);

The Read and Write operations are traced and analyzed during reachability testing.
If any Read and Write operations are involved in a data race, i.e., the shared
object is accessed outside of a critical section, then reachability testing ends
and the SYN-sequence that contained the data race is displayed.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar SharedVariables.java
   Unix:      javac -classpath .:../ModernMultithreading.jar SharedVariables.java

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar SharedVariables
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar SharedVariables

This will produce 3 files:
- ThreadID.txt: Thread IDs that were generated 
- semaphoreID.txt: semaphore names that were generated
- semaphore-replay.txt: simple PV-sequence that can be used to replay the program

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar SharedVariables
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar SharedVariables

When the sequence in semaphore-replay.txt has been replayed, you will see: Sequence Completed

4. Run it in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver SharedVariables
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver SharedVariables

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Wed Aug 01 08:42:42 GMT-05:00 2007
1/1

Reachability Testing completed.
  Executions:6 / Sequences Collected:6
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

The 6 executions correspond to the 6 possible orders that the 2 Reader and 1 Writer
threads can enter the critical section,

5. Program SharedVariables is correct in that accesses to the shared variables
   are properly protected by semaphores. Create a data race by commenting out the 
   mutex.P() and mutex.V() operations in the Writer thread:
   
      //mutex.P();
      ...
      //mutex.V();

   Run SharedVariables in reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver SharedVariables
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver SharedVariables

   The output indicates that a data race occurred:

     start:Wed Aug 01 08:52:28 GMT-05:00 2007
 
     Data race detected in the following sequence:
     PREFIX INDEX: [0, 0, 0, 0, 0, 0, 0, 0, 0]
     (7,5,1,1,mutex:P,-1,asynch_send,false,'mutex:P[S],[],semaphore_call,false,-1,-1,-1,-1,null,null,null)[0,0,0,0,0,0,1][]
     (6,5,1,3,mutex:P,-1,asynch_send,false,'mutex:P[S],[],semaphore_call,false,-1,-1,-1,-1,null,null,null)[0,0,0,0,0,1][]
     (8,1,1,1,s:Write,-1,asynch_send,false,'s:Write1[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[0,0,0,0,0,0,0,1][]
     (8,1,1,1,s:Write,-1,asynch_receive,true,s:Write1[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[1,0,0,0,0,0,0,1][]
     (7,5,1,1,mutex:P,-1,asynch_receive,true,main_binarySemaphore1:P[R],[(P,-1)],semaphore_completion,false,-1,-1,-1,-1,null,null,null)[0,0,0,0,1,0,1][]
     (7,1,2,2,s:Read,-1,asynch_send,false,'s:Read0[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[0,0,0,0,1,0,2][]
     (7,1,2,2,s:Read,-1,asynch_receive,true,s:Read1[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[2,0,0,0,1,0,2,1][]
     (8,2,2,1,b:Write,-1,asynch_send,false,'b:Writefalse[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[1,0,0,0,0,0,0,2][]
     (7,2,3,2,b:Write,-1,asynch_send,false,'b:Writetrue[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[2,0,0,0,1,0,3,1][]
     (8,2,2,1,b:Write,-1,asynch_receive,true,b:Writefalse[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[1,1,0,0,0,0,0,2][]
     (8,3,3,1,d:Write,-1,asynch_send,false,'d:Write3.0[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[1,1,0,0,0,0,0,3][]
     (7,2,3,2,b:Write,-1,asynch_receive,true,b:Writetrue[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[2,2,0,0,1,0,3,2][]
     (7,3,4,2,d:Write,-1,asynch_send,false,'d:Write2.0[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[2,2,0,0,1,0,4,2][]
     (8,3,3,1,d:Write,-1,asynch_receive,true,d:Write3.0[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[1,1,1,0,0,0,0,3][]
     (8,4,4,1,l:Write,-1,asynch_send,false,'l:Write3[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[1,1,1,0,0,0,0,4][]
     (7,3,4,2,d:Write,-1,asynch_receive,true,d:Write2.0[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[2,2,2,0,1,0,4,3][]
     (8,4,4,1,l:Write,-1,asynch_receive,true,l:Write3[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[1,1,1,1,0,0,0,4][]
     (7,4,5,2,l:Write,-1,asynch_send,false,'l:Write2[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[2,2,2,0,1,0,5,3][]
     (7,4,5,2,l:Write,-1,asynch_receive,true,l:Write2[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[2,2,2,2,1,0,5,4][]
     (7,5,6,2,mutex:V,-1,asynch_send,false,'mutex:V[S],[],semaphore_call,false,-1,-1,-1,-1,null,null,null)[2,2,2,2,1,0,6,4][]
     (7,5,6,2,mutex:V,-1,asynch_receive,true,mutex:V[R],[(V,-1)],semaphore_completion,false,-1,-1,-1,-1,null,null,null)[2,2,2,2,2,0,6,4][]
     (6,5,1,3,mutex:P,-1,asynch_receive,true,mutex:P[R],[(P,-1)],semaphore_completion,false,-1,-1,-1,-1,null,null,null)[2,2,2,2,3,1,6,4][]
     (6,1,2,3,s:Read,-1,asynch_send,false,'s:Read1[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[2,2,2,2,3,2,6,4][]
     (6,1,2,3,s:Read,-1,asynch_receive,true,s:Read1[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[3,2,2,2,3,2,6,4][]
     (6,2,3,3,b:Write,-1,asynch_send,false,'b:Writetrue[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[3,2,2,2,3,3,6,4][]
     (6,2,3,3,b:Write,-1,asynch_receive,true,b:Writetrue[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[3,3,2,2,3,3,6,4][]
     (6,3,4,3,d:Write,-1,asynch_send,false,'d:Write2.0[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[3,3,2,2,3,4,6,4][]
     (6,3,4,3,d:Write,-1,asynch_receive,true,d:Write2.0[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[3,3,3,2,3,4,6,4][]
     (6,4,5,3,l:Write,-1,asynch_send,false,'l:Write2[S],[],sharedvariable_call,false,-1,-1,-1,-1,null,null,null)[3,3,3,2,3,5,6,4][]
     (6,4,5,3,l:Write,-1,asynch_receive,true,l:Write2[R],[(Write,-1),(Read,-1)],sharedvariable_completion,false,-1,-1,-1,-1,null,null,null)[3,3,3,3,3,5,6,4][]
     (6,5,6,4,mutex:V,-1,asynch_send,false,'mutex:V[S],[],semaphore_call,false,-1,-1,-1,-1,null,null,null)[3,3,3,3,3,6,6,4][]
     (6,5,6,4,mutex:V,-1,asynch_receive,true,mutex:V[R],[(V,-1)],semaphore_completion,false,-1,-1,-1,-1,null,null,null)[3,3,3,3,4,6,6,4][]

     1/1
   
     The sequence shows that P, V, Read, and Write operations that were executed, which is helpful in 
     determining why the data race occurred.

     Note that if the shared object classes are not used and data races occur during reachability testing 
     then reachability testing will usually fail and display a message indicating that a particular
     sequence caused the failure. This message is not very helpful in determining how to fix the program.
     If the shared object classes are used, then the Read or Write operations involved in the data race
     will be displayed,helping the programmer find and fix the erroneous critical section.
