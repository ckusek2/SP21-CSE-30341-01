# SP21-CSE-30341-01

Group members:

  Calvin Kusek - ckusek2@nd.edu
  Rafael Mendizabal - rmendiza@nd.edu
  Dalton Dove - ddove@nd.edu
  
Notes:
Our hash table has a size of 256, because of this, accuracy is lost on larger files with many packets. 
We also adapted a first come first serve strategy for storing hash values of the packets in the hash table. This may lead to consumers filling up the hash table differently, which means that the number of hits will very slightly vary. 
To find the optimal number of threads for our program, we used the time command and executed it for all possible number of threads. We found that 2 (one producer, one consumer) was the optimal value of threads. This could be because of the FCFS strategy and the fact that multiple consumer threads could potentially block each other.  
