#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	char command[10];
	char parameter1[1000];
	char parameter2[1000];
	char parameter3[1000];
	char parameter4[1000];

	while(1){
		printf("ndshell> ");
		scanf("%s %s %s %s %s",command, parameter1, parameter2, parameter3, parameter4);
		//printf("Entered %s with parameters %s, %s, %s, %s\n",command, parameter1, parameter2, parameter3, parameter4);
		//TODO
	}
}
