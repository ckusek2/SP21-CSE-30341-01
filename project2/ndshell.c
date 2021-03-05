#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <sys/signal.h>
#include <time.h>

// Declare functions
void commandHandler(char *command);
void start(char *command);
void waitC();
void waitfor(char *command);
void run(char *command);
void killC(char *command);
void bound(char *command);

int main(int argc, char *argv[]){
	
	char commandline[1000];
	char *command;

	// Loop over input
	int run = 1;
	while(run){
		printf("ndshell> ");
		fgets(commandline, 1000, stdin); // Read full line of input

		commandline[strcspn(commandline, "\n")] = '\0'; // Remove new line character 
		
		if(!(command = strtok(commandline, " "))) { // Make sure input can be tokenized
			continue;
		}

		if(strcmp(command, "exit") == 0)	// Exit loop if user enters "exit"
			run = 0;
		else
			commandHandler(command);
	}
}

void commandHandler(char *command){
	
	if(strcmp(command,"start") == 0)
		start(command);
	else if(strcmp(command,"wait") == 0)
		waitC();
	else if(strcmp(command,"waitfor") == 0)
		waitfor(command);
	else if(strcmp(command,"run") == 0)
		run(command);
	else if(strcmp(command,"kill") == 0)
		killC(command);
	else if(strcmp(command,"bound") == 0)
		bound(command);
	else{
		printf("ndshell: unknown command: %s", command);
	}
}

void start(char *command){
		
	// Get command name
	command = strtok(NULL, " ");
	char *execCommand = command;

	// Add arguments to a null-terminated array
	char *args[1000];
	int index = 0;
	while(command != NULL){
		args[index++] = command;
		command = strtok(NULL, " ");
	}		
	args[index] = NULL;

	pid_t pid;

	if((pid = fork()) < 0){	// Fork child process
		printf("Error with fork\n");
		exit(1);
	} else if(pid == 0){	// Child process
		if(execvp(execCommand, args) < 0){	// Execute command
			printf("Error with execvp\n");
			exit(1);
		}
	} else{	// Output info from parent process
		printf("ndshell: process %d started\n", pid);
	}
}

void waitC(){
	return;	
}

void waitfor(char *command){
	
	// Get process ID
	char *tmp;
	if(!(command = strtok(NULL, " "))){ // Check for correct usage
		printf("ndshell: Enter the process ID. Usage: \"waitfor [ID]\"\n");
		return;
	}

	pid_t pid = (pid_t)strtoimax(command, &tmp, 10); 
	// Ensure a positive number was used as the pid
	if(pid <= 0){
		printf("ndshell: Enter a positive number for process ID. Usage: \"waitfor [ID]\"\n");
		return;
	}

	// Check if entered process still exists
	if(kill(pid, 0)<0){
		if(errno==ESRCH)	// Process doesn't exist
			printf("ndshell: No such process\n");
		else	// Other error
			printf("Error occurred: %s\n", strerror(errno));
	} else{		// Process does exist
		int status;
		while(wait(&status) != pid); // Wait for process to finish
		printf("ndshell: process %d exited normally with status %i\n", pid, status);
	}
}

void run(char *command){

	// Get command name
	command = strtok(NULL, " ");
	char *execCommand = command;

	// Add arguments to a null-terminated array
	char *args[1000];
	int index = 0;
	while(command != NULL){
		args[index++] = command;
		command = strtok(NULL, " ");
	}		
	args[index] = NULL;

	pid_t pid;
	int status;

	if((pid = fork()) < 0){	// Fork child process
		printf("Error with fork\n");
		exit(1);
	} else if(pid == 0){	// Child process
		if(execvp(execCommand, args) < 0){	// Execute command
			printf("Error with execvp\n");
			exit(1);
		}
	} else {
		while(wait(&status) != pid); // Wait for process to finish
	}
	
	printf("ndshell: process %d exited normally with status %i\n", pid, status);
}

void killC(char *command){
	
	// Get process ID
	char *tmp;
	if(!(command = strtok(NULL, " "))){ // Check for correct usage
		printf("ndshell: Enter the process ID. Usage: \"waitfor [ID]\"\n");
		return;
	}

	pid_t pid = (pid_t)strtoimax(command, &tmp, 10); 
	// Ensure a positive number was used as the pid
	if(pid <= 0){
		printf("ndshell: Enter a positive number for process ID. Usage: \"waitfor [ID]\"\n");
		return;
	}


	// Kill process with SIGKILL and check for errors
	if(kill(pid, SIGKILL)<0){
		if(errno==ESRCH)	// Process doesn't exist
			printf("ndshell: No such process\n");
		else	// Other error
			printf("Error occurred: %s\n", strerror(errno));
	} else{		// No errors
		printf("ndshell: process %d exited abnormally with signal 9: Killed.\n", pid);
	}

}
void bound(char *command){
	
	time_t start, current;

	// Get bound time
	command = strtok(NULL, " ");
	int boundTime = atoi(command);

	// Get command name
	command = strtok(NULL, " ");
	char *execCommand = command;

	// Add arguments to a null-terminated array
	char *args[1000];
	int index = 0;
	while(command != NULL){
		args[index++] = command;
		command = strtok(NULL, " ");
	}		
	args[index] = NULL;

	pid_t pid;
	int status;
	
	if((pid = fork()) < 0){	// Fork child process
		printf("Error with fork\n");
		exit(1);
	} else if(pid == 0){	// Child process
		if(execvp(execCommand, args) < 0){	// Execute command
			printf("Error with execvp\n");
			exit(1);
		}
	} else {
		// ATTEMPTED USING CLOCK.H, NOT WORKING AS EXPECTED. EXECUTING A SLEEP [BOUND] AND DETECTING WHEN IT'S DONE COULD BE AN ALTERNATIVE.
		start = clock();

		while(wait(&status) != pid){ // Wait for process to finish

			current = clock();
			if(((double)(current - start)) / CLOCKS_PER_SEC > (double)boundTime ){

				// Kill process with SIGKILL and check for errors
				if(kill(pid, SIGKILL)<0){
					if(errno==ESRCH)	// Process doesn't exist
						printf("ndshell: No such process\n");
					else	// Other error
						printf("Error occurred: %s\n", strerror(errno));
				} else{		// No errors
					printf("ndshell: process %d exceeded the time limit, killing it...\n", pid);
					printf("ndshell: process %d exited abnormally with signal 9: Killed.\n", pid);
				}

				return;
			}
		}
	}
	
	printf("ndshell: process %d exited normally with status %i\n", pid, status);

	return;
}
