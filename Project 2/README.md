Author: Naman Gangwani

NetID: nkg160030

Date: March 31, 2018

Class: CS 4348.002

How to compile this program (in a UNIX environment):
	
	javac Project2.java

How to run the program:
	
	java Project2 <numPatients> <numDoctors>

where <numPatients> and <numDoctors> must be integers that represent the number of patients and the number of doctors/nurses that the program will simulate the clinic with, respectively.


If either <numPatients> or <numDoctors> are not integers, an error will be thrown and the program will terminate. If <numPatients> or <numDoctors> are given a value less than 1, they will be defaulted to 1. If <numPatients> is given a value greater than 30, it will default to 30. If <numDoctors> is given a value greater than 3, it will default to 3.
