# SP21-CSE-30341-01

Group members:

  Calvin Kusek - ckusek2@nd.edu
  Rafael Mendizabal - rmendiza@nd.edu
  Dalton Dove - ddove@nd.edu
  
Notes:
Our plan was to only populate our hash table with the hash values that are calculated for each packet, so we decided to use a simple fixed array for storing all the data so we could still keep the O(1) access functionality. Since we do not implement a maximum packet size, we decided to make our table able to hold 100000 packets, to be safe not to go over the size constraint. 
We also adapted a first come first serve strategy for storing hash values of the packets in the hash table. This may lead to consumers filling up the hash table differently, which means that the number of hits will very slightly vary. 
To find the optimal number of threads for our program, we used the time command and executed it for all possible number of threads. We found that 2 (one producer, one consumer) was the optimal value of threads. This could be because of the FCFS strategy and the fact that multiple consumer threads could potentially block each other.  
