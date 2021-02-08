Program DistributedMutualExclusion is an implementation of the
distributed mutual exclusion algorithm in:

G. Ricart and A. K. Agrawala, An optimal algorithm for mutual 
exclusion in computer networks, Communications of the ACM, 24, 
1 (January), 1981, pp. 9-17.

1. Compile the program:
   Windows:   javac -classpath .;../ModernMultithreading.jar DistributedMutualExclusion.java 
   Unix:      javac -classpath .:../ModernMultithreading.jar DistributedMutualExclusion.java


2. Run it in reachability testing mode with 1 process:

    Windows:   java -Dmode=rt -classpath .;../ModernMultithreading.jar RTDriver DistributedMutualExclusion
    Unix:      java -Dmode=rt -classpath .:../ModernMultithreading.jar RTDriver DistributedMutualExclusion

File RTDriver.java is a driver program for RT that is in the library.
Reachability testing will exercise every possible partially-ordered sequence of 
synchronizations exactly once.

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
  Elapsed time in minutes: 4.2
  Elapsed time in seconds: 252.0
  Elapsed time in milliseconds: 252000

After displaying the start time, the number of executions is displayed, every 25
executions, until RT terminates. Then the number of exercised sequences and 
the elapsed time for RT is displayed. 

The actual elapsed time may be 4 seconds more or less than shown.

3. Run distributed reachability testing

Next, we will run distributed reachability testing with 1 manager and 1 worker process 
on a single machine. If the machine has only 1 CPU, reachability testing will not
run any faster. If there are 2 or more CPUs/cores, there should be some speedup.

To perform distributed reachability testing on program DistributedMutualExclusion, 
the RTDriver program is first executed as a manager by using the following command:

   java -classpath .;../ModernMultithreading.jar -Xmx400m -Dmode=rt -DmanagerWorker=manager -DnumWorkers=1 
   -DserverPort=20021 RTDriver DistributedMutualExclusion

The manager’s properties specify the number of workers and the port number on the 
server machine that the workers will be using to communicate with the manager. 

The following command is used to start the RTDriver program as worker 1:

   java -classpath .;../ModernMultithreading.jar -Xmx400m -Dmode=rt -DmanagerWorker=worker -DworkerNumber=1 
   -DserverPort=20021 -DserverIP=LocalHost RTDriver DistributedMutualExclusion

The worker’s properties specify the workers’s number, which identifies the worker, 
and the IP address and port number used to communicate with the server machine that 
is executing the manager program. Sicne the manager is on the same machine as the
worker, we use -DserverIP=LocalHost.

You may want to change the value for the maximum size of the heap -Xmx400m, depending on
the amount of available RAM. We assume the size of the RAM is 1GB, which must be shared
by the manager and worker processes (and any other processes running on your machine).

When the manager process and all the worker processes complete, the results for worker i are 
saved in file workerResultsi.txt and the file RTResults.txt contains the results for all the 
workers and the manager.

The file workerResults1.txt produced by worker 1 above contains:

	Reachability Testing completed.
	  Executions:1592 / Sequences Collected:1592
	  Elapsed time in minutes: 7.6947833333333335
	  Elapsed time in seconds: 461.687
	  Elapsed time in milliseconds: 461687

File RTResults.txt contains:

	Reachability Testing completed for Manager
	  Executions:2440 / Sequences Collected:2440
	  Elapsed time in minutes: 6.500516666666667
	  Elapsed time in seconds: 390.031
	  Elapsed time in milliseconds: 390031

	(sub)Total Executions:2440 (sub)Total Sequences Collected:2440

	Reachability Testing completed for Worker 1
	  Executions:1592 / Sequences Collected:1592
	  Elapsed time in minutes: 8.5315
	  Elapsed time in seconds: 511.89
	  Elapsed time in milliseconds: 511890

	(sub)Total Executions:4032 (sub)Total Sequences Collected:4032

The elapsed time recorded for the worker in file RTResults.txt is longer than the time recorded in 
workerResults1.txt due to some delays that are performed at the end of the manager’s execution. 
The times in the workerResults.txt files are more accurate. Actually, the times recorded in 
workerResults.txt are also a little longer than the actual time, the actual time typically being 30 
or seconds less than the recorded time.



