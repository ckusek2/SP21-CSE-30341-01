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
#include <pthread.h>
#include <stdbool.h>

// Packet variable definitions
typedef unsigned int guint32; 
typedef unsigned short guint16;
typedef signed int gint32;

// struct for holding packet info
struct PacketHolder{
	int theSize;
	char *pPayload;
};

// Global Variables
int numThreads = 2;	// starts at optimal number of threads
struct PacketHolder gBuffer[10];	// buffer for passing packet data between producer and consumers
int gIndex = 0;	// index the producer/consumers will be using to add to/take from buffer
int skip = 54; // The # of bytes to be skipped 
int fileStart;	// Index in argv where names of files start
int fileEnd; // Index in argv where names of files end
int numHits = 0;
int doneProducing = 0;
float totalBytes = 0;
int totalUsedPackages = 0;
pthread_mutex_t mutex;
pthread_cond_t readFrom;
pthread_cond_t putIn;

// hash table
uint32_t hashTable[100000];

// Hash value calculator
uint32_t computeHash(char* pData, int nSize){

	uint32_t hash = 0;
	uint32_t *pBlock = (uint32_t *) (pData + 54); // I think pData is a byte array of the info in the packet
	int nRemaining = nSize - 54;

	while(nRemaining >= 4){
		hash = hash ^ *pBlock;	// xor hash and pBlock
		pBlock = pBlock + 1;	// moves pBlock pointer over 4 bytes
		nRemaining -= 4;
	}

	return hash;
}


void PutInBuffer(struct PacketHolder ItemToPut){
	gBuffer[gIndex] = ItemToPut;
	gIndex = gIndex + 1;	// creates bounded buffer because doesn't allow gWriteSpot to go over 10
}

void ReadFromBuffer(struct PacketHolder *ReadItem){ 
	*ReadItem = gBuffer[gIndex];
	gIndex = gIndex - 1;	// same as for PutInBuffer, buffer has a size of 10 so can't go over that
}


void* producerThread(void *p){
	char **pArgs = p;

	for(int i = fileStart; i < fileEnd; i++){
		
		char *inputFile = pArgs[i];
		struct stat stats;
		int status = stat(inputFile, &stats);
		if(status < 0){ // Make sure the file exists
			printf("%s does not exist. \n", inputFile);
			exit(0);
		}
		if(!(strstr(inputFile, ".pcap"))) {	// Make sure the file is a pcap file
			printf("%s is not a .pcap file\n", inputFile);
			exit(0);
		}
		
		// Open and check file
		FILE *fp;
		fp = fopen(inputFile, "r");
		if(fp == NULL){
			printf("Unable to open %c: %s\n", *(char*)inputFile, strerror(errno));
			exit(1);	// exit
		}

		// Check magic number to make sure we are reading a pcap file
		guint32 magicNumber;
		fread((void*)&magicNumber, 4, 1, fp);
		if(magicNumber == 0xd4c3b2a1){
			printf("Not a valid pcap file \n");
			exit(1);
		}
	
		// This is how we read all the variables from the pcap global header	
		guint32 variable;
		for(int i = 0; i < 6; i++){
			if(i == 0 || i == 1)
				fread((void*)&variable, sizeof(guint16), 1, fp); // 16 bit major and minor numbers
			else if(i == 2)
				fread((void*)&variable, sizeof(gint32), 1, fp); // GMT to local correction
			else
				fread((void*)&variable, sizeof(guint32), 1, fp); // Other values
		}
	
		// Read packets
		while(!feof(fp)){	// Until EOF

			guint32 ts_sec;
			guint32 ts_usec;
			guint32 incl_len;
			guint32 origin_len;
		
			fread((void*)&ts_sec, 4, 1, fp);
			fread((void*)&ts_usec, 4, 1, fp);
			fread((void*)&incl_len, 4, 1, fp);
			fread((void*)&origin_len, 4, 1, fp);

			struct PacketHolder theHolder;

			theHolder.theSize = incl_len;
			totalBytes += incl_len;
			theHolder.pPayload = malloc(theHolder.theSize);

			// Read and store the packet contents
			fread(theHolder.pPayload, 1, theHolder.theSize, fp);
	
			if(theHolder.theSize >= 128){
				pthread_mutex_lock(&mutex);	// lock mutex
				if(gIndex == 10)
					pthread_cond_wait(&readFrom, &mutex);	// wait to see if consumer thread is reading from the buffer
				totalUsedPackages += 1;
				PutInBuffer(theHolder);	// put theHolder in the bounded buffer
				pthread_cond_broadcast(&putIn);	// broadcast to all consumer threads that theHolder has been added to the buffer
				pthread_mutex_unlock(&mutex);	// unlock mutex
			}
		}

		// Closing file that was being read
		fclose(fp);
	}
	doneProducing = 1;	
	return NULL;
}


void* consumerThread(){

	while(doneProducing == 0){
		struct PacketHolder theHolder;

		pthread_mutex_lock(&mutex);	// lock mutex
		while(gIndex == 0)
			pthread_cond_wait(&putIn, &mutex);	// wait to see if the producer thread is putting anything in buffer
		ReadFromBuffer(&theHolder);	// read a PacketHolder from the bounded buffer, pass by reference to update theHolder
		uint32_t hash = computeHash(theHolder.pPayload, theHolder.theSize);
		int key = hash % 100000;
		if(hashTable[key] == 0)
			hashTable[key] = hash;
		else if(hashTable[key] == hash)
			numHits += 1;

		pthread_cond_signal(&readFrom);	// signal to the producer thread that something has been taken from the buffer
		pthread_mutex_unlock(&mutex);	// unlock mutex
	}

	return NULL;
}


int main(int argc, char *argv[]){

	printf("Welcome to Project 3 - ThreadRE by Calvin Kusek, Dalton Dove, and Rafael Mendizabal\n");
	printf("Now operating in Level 1 mode - full payload detection\n");

	if(strcmp(argv[1], "-thread") == 0){
		numThreads = atoi(argv[2]);
		if(numThreads < 2){
			printf("Minimum of two threads required \n");
			exit(0);
		} else if(numThreads > 8){
			printf("Cannot use more than 8 threads/cores\n");
			exit(0);
		} else {
			printf("Threads allowed: %i\n", numThreads);
			fileStart = 3;	// file names start at argv[3]
		}
	} else {
		printf("Using optimal number of threads: %i\n", numThreads);
		fileStart = 1; // file names start at argv[1]
	}

	printf("Allocating 1 thread to file I/O, %i thread(s) to processing\n", numThreads - 1);
	printf("Files to process: ");
	for(int output = fileStart; output < argc; output++)
		printf("%s ", argv[output]);
	printf("\n");
	fileEnd = argc;

	for(int i = 0; i < 256; i++)
		hashTable[i] = 0;

	pthread_t producer;
	pthread_t consumer;
	pthread_create(&producer, NULL, &producerThread, argv);
	for(int i = 0; i < numThreads - 1; i++)
		pthread_create(&consumer, NULL, &consumerThread, NULL);
	pthread_join(producer, NULL);

	printf("Results:\n");
	printf("%f MB processed\n", totalBytes / 1000000);
	printf("%i hits\n", numHits);
	printf("%i packages used\n", totalUsedPackages);
	float redundancy = ((float)numHits / totalUsedPackages) * 100;
	printf("%.2f%c redundancy detected\n", redundancy, '%');
}
