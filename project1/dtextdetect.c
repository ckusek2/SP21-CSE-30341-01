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
	int totalBytes = 0;	// Total number of bytes in file
	int readBytes = 0;	// Number of human readable characters
	int result = read(fd, buffer, 4096);	// Read chunk of data from file
	while(result > 0){	// Run while still data in file
		for(int i = 0; i < result; i++){	// Count human readable bytes in chunk of data
			if(buffer[i] >= 32 && buffer[i] <= 126){
				readBytes++;
			}
		}
		totalBytes += result;
		result = read(fd, buffer, 4096);
	}

	float perc = (float)readBytes/(float)totalBytes * 100;	// % of human readable characters in file
	
	// Outputting information to user
	printf("textdetect: %s -> %.02f%% HR [%i / %i bytes]\n", filename, perc, readBytes, totalBytes);

	// Opening json file to write to
	FILE *fp = fopen("results.json", "w+");
	// Write statistics to results.json
	if(perc >= thresh){
		fprintf(fp,"[ {\"name\" : \"%s\", \"readable\" : %i, \"bytes\" : %i } ]\n", filename, readBytes, totalBytes);
	}

	// Closing results.json file
	if(fclose(fp) != 0){
		printf("Unable to close results.json: %s\n", strerror(errno));
		exit(1);
	}
	
	// Closing file that was being read
	if(close(fd) < 0){
		printf("Unable to close %s: %s\n", filename, strerror(errno));
		exit(1);	
	}
}
