Author: Naman Gangwani

NetID: nkg160030

Date: Febuary 24, 2018

Project files:

1. Project1.java - the main method; also contains a private CPU class that communicates with the memory process, fetches and executes instructions, and handles interrupts

2. Memory.java - treated as a separate process; reads input from other processes and performs those commands accordingly; handles reading of and writing to memory

How to compile:

1. Make sure you are in a UNIX environment and have both files in your current directory

2. Compile both files with this command: 
	javac Project1.java Memory.java

How to run:

Run the main method file (Project1) and provide program filename and timer interrupt value as arguments of the command in this syntax:
	
	java Project1 <program filename> <timer interrupt>

For example, if you want to run "sample5.txt" with a timer interrupt value of 30:
	
	java Project1 sample5.txt 30
