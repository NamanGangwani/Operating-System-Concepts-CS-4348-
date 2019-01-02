Author: Naman Gangwani

NetID: nkg160030

Date: April 23, 2018

Class: CS 4348.002


How to compile this program (in a UNIX environment):
	
	g++ Project3.cpp

How to run the program:
	
	./a.out <algorithm>

where <algorithm> is either FCFS, RR, SPN, SRT, HRRN, FB, or ALL. Case sensitivity matters. If an invalid <algorithm> type is entered, the program will output nothing and terminate.

There should be a file called "jobs.txt" in the same directory as Project3.cpp when compiling and running the program. The contents of jobs.txt should match the specifications in the project description:
- One line per job, where each line has a name, arrival time, and duration that are tab-delimited
- The job names must be a letter from A-Z
- The first job should be named A, and the remaining jobs should be named sequentially, so if there are five jobs, they are named A-E.
- The arrival times of the jobs should be in order
