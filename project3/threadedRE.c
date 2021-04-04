#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

// Global variables
struct PacketHolder{
	int theSize;
	char *pPayload;
	uint32_t hashValue;
	int hitCount;
};

int numCores = 8;	// need to find out optimal number of threads
struct PacketHolder gBuffer[10];	// will be 10 or less files processed
int gWriteSpot;
int gReadSpot;
// add hash table

// Got this stuff from the panopto video from 5 days ago
char PutInBuffer(struct PacketHolder ItemToPut);

void* producerThread(void* pArgs){

	// Open file and check it
	
	while(LoopToEndOfFile){	// If we use fgets, I think we can just say while fgets() != NULL

		struct PacketHolder theHolder;
		int nBytesRead;

		// I believe we need a malloc here

		nBytesRead = fread(pFile, theHolder->pPayload, ...);
		theHolder.theSize = nBytesRead;

		PutInBuffer(theHolder);
	}
}

// Got this from the panopto video from 3 days ago
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

	printf("Welcome to Project 3 - ThreadRE by Calvin Kusek, Dalton Dove, and Rafael Mendizabal");

	int fileStart;	// Index in argv where names of files start
	if(strcmp(argv[1], "-thread") == 0){
		numCores = atoi(argv[2]);
		fileStart = 3;	// file names start at argv[3]
	} else
		fileStart = 1;

	struct stat stats;
	for(int start = fileStart; fileStart < argc; start++){
		stat(argv[start], &stats);
		// I think we can use stats.st_mode to make sure the file is a .pcap file
		// If it is a pcap file, do stuff with it lol
	}
}
