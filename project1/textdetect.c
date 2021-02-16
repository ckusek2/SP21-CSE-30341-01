#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	
	// Verify input
	if(argc != 3){	// Making sure there is the correct number of args
		printf("Incorrect number of arguments...\n");
		printf("Usage: textdetect InputFile Threshold\n");
		exit(1);	// exit
	}

	float thresh = strtof(argv[2],NULL);	// Inputted threshold
	if(thresh < 0.0){	// Making sure threshold is positive
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
	
	
	// Going over file and counting human readable/nonreadable characters
	char buffer[4096];
	int totalBytes = 0;
	int readBytes = 0;
	int result = read(fd, buffer, 4096);
	while(result > 0){
		for(int i = 0; i < result; i++){
			if(buffer[i] >= 32 && buffer[i] <= 126){
				readBytes++;
			}
		}
		totalBytes += result;
		result = read(fd, buffer, 4096);
	}

	float perc = (float)readBytes/(float)totalBytes;
	printf("%f, %i, %i\n", perc, readBytes, totalBytes);

}
