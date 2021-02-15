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

	int thresh = atoi(argv[2]);	// Inputted threshold
	if(thresh < 0){	// Making sure threshold is positive
		printf("Threshold cannot be negative\n");
		return 1;	// exit
	}

	// Opening file for reading
	char *filename = argv[1];
	FILE *file = fopen(filename, "r");

	// Testing to make sure filename is an actual file
	if(file == NULL){
		printf("Couldn't open %s: No such file\n", filename);
	}


	
}
