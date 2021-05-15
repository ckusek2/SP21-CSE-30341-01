
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

int bitMap[10000] = {0};
int mounted = 0;

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

	// Checking if fs is mounted already
	if(mounted == 1)
		return 0;

	union fs_block block;
	union fs_block iNodeBlock;

	disk_read(0, block.data);

	block.super.magic = FS_MAGIC;
	block.super.nblocks = disk_size();
	block.super.ninodeblocks = 0.1 * disk_size();
	block.super.ninodes = 0.1 * disk_size() * INODES_PER_BLOCK;

	disk_write(0, block.data);

	disk_read(1, iNodeBlock.data);

	// Setting indoes on fs to invalid
	for(int i = 0; i < INODES_PER_BLOCK; i++){
		iNodeBlock.inode[i].isvalid = 0;
	}

	disk_write(1, iNodeBlock.data);
		
	return 1;
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

	// load disk block 0
	disk_read(0,block.data);

	// printing superblock information
	if(block.super.magic == FS_MAGIC){
		
		// load disk block 1
		disk_read(1, iNodeBlock.data);

		for(int i = 0; i < POINTERS_PER_INODE; i++){
			// make sure inode is valid
			if(iNodeBlock.inode[i].isvalid){	

				// Get size of inode
				int nodeSize = iNodeBlock.inode[i].size;
				int numBlock = 0;
		
				while(nodeSize > 0){
					nodeSize -= 4096;
					// Checks if it is a direct or indirect block
					if(numBlock <= 4){
						bitMap[iNodeBlock.inode[i].direct[numBlock]] = 1;
					} else {
						int indirect = iNodeBlock.inode[i].indirect;
						// check if indirect block is 0
						if(indirect){
							bitMap[indirect] = 1;
							disk_read(indirect, indirectBlock.data);
							// Loop over indirect pointers
							for(int pointer = 0; pointer < POINTERS_PER_BLOCK && nodeSize > 0; pointer++){
								nodeSize -= 4096;
								// Check if pointer is valid
								if(indirectBlock.pointers[pointer])
									bitMap[indirectBlock.pointers[pointer]] = 1;
							}
						}		
					}
					numBlock += 1;
				}
			}
		}
	} else { return 0; } // filesystem not present, fail

	mounted = 1;
	return 1;
}

int fs_create()
{

	// Make sure filesystem is mounted
	if(mounted == 0)
		return 0;

	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;	

	// load disk block 0
	disk_read(0,block.data);

	// Make sure inode can be created
	if(block.super.magic == FS_MAGIC){

		disk_read(1, iNodeBlock.data);

		// Go through inodes and find available one
		for(int i = 1; i < POINTERS_PER_INODE; i++){
			
			// If inode is not valid, create it with zero length
			if(!iNodeBlock.inode[i].isvalid){

				iNodeBlock.inode[i].isvalid = 1;
				iNodeBlock.inode[i].size = 0;
				disk_write(1, iNodeBlock.data);
				return i;
			}
		}
	}

	return 0;
}

int fs_delete( int inumber )
{

	// Make sure filesystem is mounted
	if(mounted == 0)
		return 0;

	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;	

	// load disk block 0
	disk_read(0,block.data);

	// Make sure inode can be deleted
	if(block.super.magic == FS_MAGIC){

		disk_read(1, iNodeBlock.data);
			
		// Make sure inode is valid for deletion
		if(iNodeBlock.inode[inumber].isvalid){

			iNodeBlock.inode[inumber].isvalid = 0;
			iNodeBlock.inode[inumber].size = 0;
			iNodeBlock.inode[inumber].indirect = 0;
			// Make all direct blocks and bitmap for those blocks zero
			for(int i = 0; i < POINTERS_PER_INODE; i++){
				
				bitMap[iNodeBlock.inode[inumber].direct[i]] = 0;
				iNodeBlock.inode[inumber].direct[i] = 0;

			}
			disk_write(1, iNodeBlock.data);
			return 1;
		} 
	}

	return 0;
}

int fs_getsize( int inumber )
{

	// Make sure filesystem is mounted
	if(mounted == 0)
		return -1;

	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;	

	// load disk block 0
	disk_read(0,block.data);

	// Make sure inode can be read
	if(block.super.magic == FS_MAGIC){

		disk_read(1, iNodeBlock.data);
			
		// Make sure inode is valid for reading
		if(iNodeBlock.inode[inumber].isvalid){
			return iNodeBlock.inode[inumber].size;
		}
	}

	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{

	// Make sure filesystem is mounted
	if(mounted == 0)
		return -1;

	// declaring unions
	union fs_block block;
	union fs_block iNodeBlock;	
	union fs_block inode;
	union fs_block indirectBlock;

	// declaring reference variables
	int len = length;
	int start = 0;
	int data_ptr = 0;
	int sofar = 20000;


	// load disk block 0
	disk_read(0,block.data);

	// Make sure inode can be read
	if(block.super.magic == FS_MAGIC){

		disk_read(1, iNodeBlock.data);
			
		// Make sure inode is valid for reading
		if(iNodeBlock.inode[inumber].isvalid){
			// Make sure offset starts in direct nodes
			if(offset < POINTERS_PER_INODE * 4000){
				for(int i = 0; i < POINTERS_PER_INODE; i++){
					disk_read(iNodeBlock.inode[inumber].direct[i], inode.data);
					for(int j = i * 4000; j < ((i+1)*4000); j++){
						if(j == offset) 
							start = 1;
						if(start){
							data[data_ptr] = inode.data[j%4000];
							data_ptr++;
							len -= 1;
							if(len <= 0)
								start = 0;
						}
					}
				}
			}
			int indirect = iNodeBlock.inode[inumber].indirect; 
			if(indirect && len > 0){
				disk_read(indirect, indirectBlock.data);
				// go through pointers on indirect block and print non-zero pointer values
				for(int pointer = 0; pointer < POINTERS_PER_BLOCK; pointer++){
					if(indirectBlock.pointers[pointer]){
						for(int i = 0; i < 4000; i++){
							if(sofar == offset)
								start = 1;
							if(start){
								data[data_ptr] = indirectBlock.data[sofar%4000];
								data_ptr++;
								len -= 1;
								if(len <= 0)
									start = 0;
							}
							sofar += 1;
						}
					}
				}

			}
		}

	}

	return data_ptr;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	return 0;
}
