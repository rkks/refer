Program DiningPhilosophers is a semaphore solution to the 
dining philosophers problem with 4 philosophers.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar  DiningPhilosophers.java
   Unix:      javac -classpath .:../ModernMultithreading.jar  DiningPhilosophers.java  

2. Run it in trace mode:
   Windows:   java -Dmode=trace -classpath .;../ModernMultithreading.jar  DiningPhilosophers
   Unix:      java -Dmode=trace -classpath .:../ModernMultithreading.jar  DiningPhilosophers

This will produce 3 files:
- ThreadID.txt: Thread IDs that were generated 
- semaphoreID.txt: semaphore names that were generated
- semaphore-replay.txt: simple PV-sequence that can be used to replay the program

To turn deadlock detection on in trace mode, use:
   Windows:   java -Dmode=trace -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  DiningPhilosophers
   Unix:      java -Dmode=trace -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  DiningPhilosophers

If you run the program in trace mode with deadlockDetection on you may or may not
detect a deadlock. Here's what it will look like if you do:

  Philosopher # 3  Running
  Philosopher # 2  Running
  Philosopher # 0  Running
  Philosopher # 1  Running

  Monitoring for Deadlock.
  Deadlock detected:
  - Philosopher2 blocked on P operation of stick3
  - Philosopher0 blocked on P operation of stick1
  - Philosopher3 blocked on P operation of stick0
  - Philosopher1 blocked on P operation of stick2

  Execution trace:
  Thread Philosopher1 completed stick1.P()
  Thread Philosopher0 completed stick0.P()
  Thread Philosopher3 completed stick3.P()
  Thread Philosopher2 completed stick2.P()
  Thread Philosopher1 blocking on stick2.P()
  Thread Philosopher2 blocking on stick3.P()
  Thread Philosopher3 blocking on stick0.P()
  Thread Philosopher0 blocking on stick1.P()

To execute random delays during trace mode, set property -DrandomDelay=on. Random delays will 
be executed at the beginning of each thread’s execution, and at the beginning of P and V operations.
This may help you to exercise a deadlock scenario.

3. Run it in replay mode:
   Windows:   java -Dmode=replay -classpath .;../ModernMultithreading.jar  DiningPhilosophers
   Unix:      java -Dmode=replay -classpath .:../ModernMultithreading.jar  DiningPhilosophers

When the sequence in semaphore-replay.txt has been replayed, you will see: Sequence Completed

4. Run it in reachability testing (RT) mode with deadlock detection on:

    Windows:   java -Dmode=rt -DdeadlockDetection=on -classpath .;../ModernMultithreading.jar  RTDriver DiningPhilosophers
    Unix:      java -Dmode=rt -DdeadlockDetection=on -classpath .:../ModernMultithreading.jar  RTDriver DiningPhilosophers

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Mon Dec 05 20:36:20 GMT-05:00 2005
Monitoring for Deadlock ....
Deadlock detected:
- Philosopher2 blocked on P operation of stick3
- Philosopher0 blocked on P operation of stick1
- Philosopher3 blocked on P operation of stick0
- Philosopher1 blocked on P operation of stick2

To remove the deadlock change the line:

  Philosopher p3 = new Philosopher (stick3, stick0, 3);

to

  Philosopher p3 = new Philosopher (stick0, stick3, 3);

Now when you run in -Dmode=rt, no deadlock will be detected and 14
sequences will be collected. (There may be 14 or 15 executions, but
only 14 sequences will be collected.)

The actual elapsed time may be 4 seconds more or less than shown.
