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

// Declare functions
void commandHandler(char *command);
void start(char *command);
void waitC(char *command);
void waitfor(char *command);
void run(char *command);
void killC(char *command);
void bound(char *command);

int main(int argc, char *argv[]){
	
	char commandline[1000];
	char *command;

	// Loop over input
	while(1){
		printf("ndshell> ");
		scanf("%[^\n]%*c",commandline); // Read full line of input.
		command = strtok(commandline, " "); // Split string into tokens. 
		commandHandler(command);
	}
}

void commandHandler(char *command){
	
	if(strcmp(command,"start") == 0)
		start(command);
	else if(strcmp(command,"wait") == 0)
		waitC(command);
	else if(strcmp(command,"waitfor") == 0)
		waitfor(command);
	else if(strcmp(command,"run") == 0)
		run(command);
	else if(strcmp(command,"kill") == 0)
		killC(command);
	else if(strcmp(command,"bound") == 0)
		bound(command);
	else{
		printf("ndshell: unknown command: %s\n", command);
		return;
	}
	return;
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
	} else {
		printf("%i\n", pid);
	}

	return;
}

void waitC(char *command){
	return;
}
void waitfor(char *command){
	
	// Get process ID
	char *tmp;
	command = strtok(NULL, " ");
	pid_t pid = (pid_t)strtoimax(command, &tmp, 10);
	
	int status;

	while(wait(&status) != pid); // Wait for process to finish

	return;
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

	return;
}
void killC(char *command){
	return;
}
void bound(char *command){
	return;
}
