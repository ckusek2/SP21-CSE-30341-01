#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	
	// Verify input
	if(argc != 3){	// Making sure there is the correct number of args
		printf("Incorrect number of arguments...\n");
		printf("Usage: textdetect InputFile Threshold\n");
		return 1;	// exit
	}

	if(atoi(argv[2]) < 0){	// Making sure threshold is positive
		printf("Threshold cannot be negative\n");
		return 1;	// exit
	}


	
}
