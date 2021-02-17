#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// Initialize directory, byte, and file counting variables
int dirCount;
int fileCount;
int dTotalBytes;

// results.json file
FILE *fp;

int main(int argc, char *argv[]){
	
	// Verify input
	if(argc != 3){	// Making sure there is the correct number of args
		printf("Incorrect number of arguments...\n");
		printf("Usage: dtextdetect SourcePath Threshold\n");
		exit(1);	// exit
	}

	float thresh = strtof(argv[2],NULL);	// Inputted threshold
	if(thresh < 0.0){	// Making sure threshold is positive
		printf("Threshold cannot be negative\n");
		exit(1);	// exit
	}

	// directory path
	char *pPath = argv[1];

	dirCount = 0;
	fileCount = 0;
	dTotalBytes = 0;

	// Opening json file to write to
	fp = fopen("results.json", "w+");

	// Use listAllDirs to go through directories
	listAllDirs(pPath);

	// Print dtextdetect results
	printf("dtextdetect: Examined %i directories, %i files, and %i bytes from %s", dirCount, fileCount, dTotalBytes, filename);

	// Closing results.json file
	if(fclose(fp) != 0){
		printf("Unable to close results.json: %s\n", strerror(errno));
		exit(1);
	}

}
	
void listAllDirs(char *pPath){

	DIR *dirp;
	struct dirent *dp;

	dirp = opendir(pPath);

	if(dirp == NULL){

		char szTemp[256];

		sprintf(szTemp, "Could not open %s", pPath);
		perror(szTemp);
		return;
	}

	// Loop using readdir until the result is NULL
	while((dp = readdir(dirp)) != NULL){
		if((dp = readdir(dirp)) != NULL){
			
			// Get file information		
			stat(dp->d_name, &sb);

			if((sb.st_mode & S_IFMT) == S_ISDIR){
				listAllDirs(dp->d_name);
			}				
		}
	}
}

	
String readFile(char *filename){
	
	int fd = open(filename, O_RDONLY,0);

	// Testing to make sure filename is an actual file
	if(fd < 0){
		printf("Unable to open %s: %s\n", filename, strerror(errno));
		exit(1);	// exit
	}
	
	
	// Going over file and counting human readable/nonreadable characters	
	char buffer[4096];
	int fTotalBytes = 0;	// Total number of bytes in file
	int readBytes = 0;	// Number of human readable characters

	int result = read(fd, buffer, 4096);	// Read chunk of data from file
	while(result > 0){	// Run while still data in file
		for(int i = 0; i < result; i++){	// Count human readable bytes in chunk of data
			if(buffer[i] >= 32 && buffer[i] <= 126){
				readBytes++;
			}
		}
		fTotalBytes += result;
		result = read(fd, buffer, 4096);
	}

	float perc = (float)readBytes/(float)fTotalBytes * 100;	// % of human readable characters in file
	dTotalBytes += fTotalBytes;	// Adding up bytes from all files in directory

	// Outputting information to user
	printf("textdetect: %s -> %.02f%% HR [%i / %i bytes]\n", filename, perc, readBytes, fTotalBytes);

	// Write statistics to results.json
	if(perc >= thresh){
		return ("[ {\"name\" : \"%s\", \"readable\" : %i, \"bytes\" : %i } ]\n", filename, readBytes, fTotalBytes);
	}
		
	// Closing file that was being read
	if(close(fd) < 0){
		printf("Unable to close %s: %s\n", filename, strerror(errno));
		exit(1);	
	}
}
