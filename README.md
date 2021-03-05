# SP21-CSE-30341-01

Group members:

  Calvin Kusek - ckusek2@nd.edu
  Rafael Mendizabal - rmendiza@nd.edu
  Dalton Dove - ddove@nd.edu

We decided to split up the project a little more for project2; however, we did not give specific functions and parts to specific members of the group. Instead, we decided to just let everyone decide what they wanted to work on and made sure that there was not any over-lapping work. Also, some group members worked on the same functions until they were completed fully. For example, one member might have started working on the kill() function and gotten to a good point with it, but another group member then finished it up and made sure it worked well. We thought this strategy would work well because, before we started the project, we were not completely sure which functions would require the most work and which funtions would be easier to finish. Also, the ability to get different viewpoints and thoughts on how to carry out a function was very helpful.

List of commands:
start {command}         Non-blocking,
wait                    Blocking,
waitfor {PID}           Blocking,
run {command}           Blocking,
kill {PID}              Non-blocking,
bound {time} {command}  Blocking

For bound, we decided to not allow the user to enter a bound time of 0 seconds, as it was a little confusing on how to handle this and the process wouldn't really make sense. Forcing the user to enter a positive number greater than 1 makes things much simpler and run better.
