#pragma once


#define DISCNAME_LENGTH 16

#define EMPTY 0
#define UNUSED -1
#define USED 1

typedef struct
{
	unsigned int blocksNumber;
	unsigned int freeBlocksNumber;
	unsigned int nodesNumber;
	unsigned int freeNodesNumber;

	char name[DISCNAME_LENGTH];
	unsigned long size;

	unsigned long nodesOffset;
	unsigned long bitVectorOffset;
	unsigned long blocksOffset;
} SuperBlock;


#define BLOCK_SIZE 256

typedef struct
{
	unsigned char data[BLOCK_SIZE];
	unsigned long nextBlock;
} Block;


SuperBlock NewSuperBlockInstance(unsigned long size);
int GetFirstFreeBlockIndex();
void SetBlockState(int index, int state);
int GetFirstFreeNodeIndex();
void DeleteNode(char* filename);