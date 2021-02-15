#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	// Verify input
	if(argc != 3){	// Making sure there is the correct number of args
		printf("Incorrect number of arguments...\n");
		printf("Usage: textdetect InputFile Threshold\n");
		exit(1);	// exit
	}

	int thresh = atoi(argv[2]);	// Inputted threshold
	if(thresh < 0){	// Making sure threshold is positive
		printf("Threshold cannot be negative\n");
		exit(1);	// exit
	}

	// Opening file for reading
	char *filename = argv[1];
	int fd = open(filename, O_RDONLY,0);

	// Testing to make sure filename is an actual file
	if(fd < 0){
		printf("Unable to open %s: %s\n", filename, strerror(errno));
		exit(1);	// exit
	}

	
	
}
