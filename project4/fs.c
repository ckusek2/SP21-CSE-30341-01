
#include "fs.h"
#include "disk.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024

struct fs_superblock {
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};

struct fs_inode {
	int isvalid;
	int size;
	int direct[POINTERS_PER_INODE];
	int indirect;
};

union fs_block {
	struct fs_superblock super;
	struct fs_inode inode[INODES_PER_BLOCK];
	int pointers[POINTERS_PER_BLOCK];
	char data[DISK_BLOCK_SIZE];
};

int fs_format()
{
	return 0;
}

void fs_debug()
{
	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;
	union fs_block indirectBlock;

	// load disk block 0
	disk_read(0,block.data);

	// printing superblock information
	printf("superblock:\n");
	if(block.super.magic == FS_MAGIC)
		printf("    magic number is valid\n");
	printf("    %d blocks on disk\n",block.super.nblocks);
	printf("    %d blocks for inodes\n",block.super.ninodeblocks);
	printf("    %d inodes total\n",block.super.ninodes);

	// load disk block 1
	disk_read(1, iNodeBlock.data);

	for(int i = 0; i < POINTERS_PER_INODE; i++){
		// make sure inode is valid
		if(iNodeBlock.inode[i].isvalid){
			// print inode number and size
			printf("inode %d:\n", i);
			printf("    size: %d bytes\n", iNodeBlock.inode[i].size);
			
			// print out the direct blocks in iNodeBlock
			printf("    direct blocks: ");
			for(int j = 0; j < POINTERS_PER_INODE; j++){
				if(iNodeBlock.inode[i].direct[j])
					printf("%d ", iNodeBlock.inode[i].direct[j]);
			}
			printf("\n");


			int indirect = iNodeBlock.inode[i].indirect;
			// check if indirect block is 0
			if(indirect){
				// print indirect block number
				printf("    indirect block: %d\n", iNodeBlock.inode[i].indirect);

				// print indirect data blocks
				printf("    indirect data blocks: ");
				// load disk block of indirect disk
				disk_read(indirect, indirectBlock.data);
				// go through pointers on indirect block and print non-zero pointer values
				for(int pointer = 0; pointer < POINTERS_PER_BLOCK; pointer++){
					if(indirectBlock.pointers[pointer])
						printf("%d ", indirectBlock.pointers[pointer]);
				}
				printf("\n");
			}
		}
	}
}

int fs_mount()
{
	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;
	union fs_block indirectBlock;
	int bitMap[SIZE_MAX] = {0};

	// load disk block 0
	disk_read(0,block.data);

	// printing superblock information
	if(block.super.magic == FS_MAGIC){
		
		// load disk block 1
		disk_read(1, iNodeBlock.data);

		for(int i = 0; i < POINTERS_PER_INODE; i++){
			// make sure inode is valid
			if(iNodeBlock.inode[i].isvalid){
				
				int nodeSize = iNodeBlock.inode[i].size;
				int numBlock = 0;
				while(nodeSize > 0){
					nodeSize -= 4096;
					if(numBlock <= 4){
						bitMap[iNodeBlock.inode[i].direct[numBlock]] = 1;
					} else {
						//indirect suff


			int indirect = iNodeBlock.inode[i].indirect;
			// check if indirect block is 0
			if(indirect){
				// print indirect block number
				printf("    indirect block: %d\n", iNodeBlock.inode[i].indirect);

				// print indirect data blocks
				printf("    indirect data blocks: ");
				// load disk block of indirect disk
				disk_read(indirect, indirectBlock.data);
				// go through pointers on indirect block and print non-zero pointer values
				for(int pointer = 0; pointer < POINTERS_PER_BLOCK; pointer++){
					if(indirectBlock.pointers[pointer])
						printf("%d ", indirectBlock.pointers[pointer]);
				}
				printf("\n");
			}
		}
	}
	return 0;
}

int fs_create()
{
	return 0;
}

int fs_delete( int inumber )
{
	return 0;
}

int fs_getsize( int inumber )
{
	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{
	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}
