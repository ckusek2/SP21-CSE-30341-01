# SP21-CSE-30341-01
Project 4: File Systems

Group members:

  Calvin Kusek - ckusek2@nd.edu
  Rafael Mendizabal - rmendiza@nd.edu

For this project, we both worked on the project at the same time, where one person would be the driver and the other person would contribute ideas. We both worked together to come up with a solution because we thought it would be more efficient and beneficial to bounde ideas off of each other and work on each function together.

We added an extra outside loop to fs_debug to go through and read from all of the blocks, which we believe will possibly help in some edge cases and other tests.
For fs_read(), we believe we are getting the correct output, but for some reason there is some extra data that gets printed even after we have already returned from the function. Also, in some other instances, some question marks show up between the numbers of the total bytes read. For example, instead of 4420000, we get 4???4???20000. 

List of commands given by shell.c:
  format
  mount
  debug
  create
  delete  <inode>
  cat     <inode>
  copyin  <file> <inode>
  copyout <inode> <file>
  help
  quit
  exit
