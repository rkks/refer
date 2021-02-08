Program DMEMonitorSC is an implementation of the distributed mutual exclusion algorithm in:

G. Ricart and A. K. Agrawala, An optimal algorithm for mutual exclusion in computer networks, Communications of the ACM, 24, 
1 (January), 1981, pp. 9-17.

There are two processes.

1. Run it in stateless reachability testing (RT) mode.

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver DMEMonitorSC
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver DMEMonitorSC

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible sequence exactly once.

The output is:

start:Wed Mar 13 09:05:25 EDT 2013
1/1
25/25
50/50
75/75

Reachability Testing completed.
  Executions:96 / Sequences Collected:96
  Elapsed time in minutes: 0.06666666666666667
  Elapsed time in seconds: 4.0
  Elapsed time in milliseconds: 4000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

2. Run stateful reachability testing:

    Windows:   java -Dmode=rt -Dhash=25 -DstatefulRT=on -DstatePruning=on -DmaxThreads=10 -classpath .;../ModernMultithreading.jar RTDriverStateful DMEMonitorSC
    Unix:      java -Dmode=rt -Dhash=25 -DstatefulRT=on -DstatePruning=on -DmaxThreads=10 -classpath .:../ModernMultithreading.jar RTDriverStateful DMEMonitorSC

Stateful reachability testing extracts and stores program states to reduce the number of sequences that are executed 
during reachability testing. To perform stateful reachability testing you must use the stateful RT driver. Stateful 
reachability testing is performed in "rt" mode -Dmode=rt with the property -DstatefulRT=on. 

The output of stateful reachability testing for DMEMonitorSC is as follows:
start:Tue Mar 12 14:07:09 EDT 2013
start input
1/1
25/25

Reachability Testing completed.
  Executions:42 / Sequences Collected:42
  Peak Stored States: 16
  Number of States: 336
  Number Variants Pruned: 32
  Elapsed time in minutes: 0.025
  Elapsed time in seconds: 1.5
  Elapsed time in milliseconds: 1500

After displaying the start time, the number of executions is displayed, every 25 executions, until stateful 
RT terminates. The information displayed after RT completes is:

- Then the number of exercised sequences, 
- The peak stored states, which is the maximum number of program states that had to be stored in memory at any one time
- The number of states in the program
- The number of variants that did not have to be exercised because these variants would only visit a part of the 
  state space that had been visited before.
- The elapsed time for RT. The actual elapsed time may be 4 seconds more or less than shown. 

Regular RT exercises 96 sequences for this program. Thus, stateful RT reduces the number of executions, at the cost 
of more space for storing states, and additional time and effort for extracting states.

