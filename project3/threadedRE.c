#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

typedef unsigned int guint32; 
typedef unsigned short guint16;

struct PacketHolder{
	int theSize;
	char *pPayload;
	uint32_t hashValue;
	int hitCount;
};

// Global Variables
int numCores = 8;	// need to find out optimal number of threads
struct PacketHolder gBuffer[10];	// will be 10 or less files processed
int gWriteSpot;
int gReadSpot;
int skip = 54; // The # of bytes to be skipped 

// add hash table

// Got this stuff from the panopto video from 5 days ago
char PutInBuffer(struct PacketHolder ItemToPut);

void* producerThread(void* pArgs){

	// Open file and check it
	FILE *fp;
	char str[54];

	fp = fopen(pArgs, "r");

	if(fp == NULL){
		printf("Unable to open %s: %s\n", pArgs, strerror(errno));
		exit(1);	// exit
	}
	
	// I think we need to switch this up so it's more "bits-oriented"
	//while(fgets(str, 54, fp) != NULL){	// If we use fgets, I think we can just say while fgets() != NULL

		//struct PacketHolder theHolder;
		//int nBytesRead;

		// I believe we need a malloc here

		//nBytesRead = fread(pFile, theHolder->pPayload, ...);
		//theHolder.theSize = nBytesRead;

		//PutInBuffer(theHolder);
	//}
	
	// This is how we read all the variables from the pcap global header starting with magic number
	guint32 variable;
	for(int i = 0; i < 10; i++){
		fread((void*)&variable, 4, 1, fp);
		printf("%x\n", variable);
	}

	// Closing file that was being read
	fclose(fp);
	return 1;
}

// Hash value calculator
uint32_t computeHash(char* pData, int nSize){

	uint32_t hash = 0;
	uint32_t *pBlock = (uint32_t *) (pData + 54); // I think pData is a byte array of the info in the packet
	int nRemaining = nSize - 54;

	while(nRemaining > 0){
		hash = hash ^ *pBlock;	// xor hash and pBlock
		pBlock = pBlock + 1;	// moves pBlock pointer over 4 bytes
		nRemaining -= 4;

		if(nRemaining < 4) // not 4 bytes to do xor with
			break;
	}

	return hash;
}

// I think we also need to make a thread for input/output
int main(int argc, char *argv[]){

	printf("Welcome to Project 3 - ThreadRE by Calvin Kusek, Dalton Dove, and Rafael Mendizabal \n");

	int fileStart;	// Index in argv where names of files start
	if(strcmp(argv[1], "-thread") == 0){
		numCores = atoi(argv[2]);
		fileStart = 3;	// file names start at argv[3]
	} else
		fileStart = 1;

	struct stat stats;
	int status;

	for(int start = fileStart; start < argc; start++){
		status = stat(argv[start], &stats);
		if(status < 0) // Make sure the file exists. For now we can assume we are actually gonna give it a .pcap file.
			printf("%s does not exist. \n", argv[start]);
		// I think we can use stats.st_mode to make sure the file is a .pcap file. EDIT: We can just check the string for .pcap (if it exists).
		// If it is a pcap file, do stuff with it lol
		producerThread(argv[start]);
	}
}
