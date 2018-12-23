
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Disc.h"
#include "SuperBlock.h"
#include "Node.h"

Disc NewDiscInstance(unsigned int size)
{
	Disc disc;
	disc.superBlock = NewSuperBlockInstance(size);
	return disc;
}

int OpenDiscFile(char* filename)
{
	disc.file = fopen(filename, "rb+");
	if (!disc.file) return 0;

	fread(&disc.superBlock, sizeof(SuperBlock), 1, disc.file);

	return 1;
}

int CreateDiscFile(char* filename)
{
	strcpy(disc.superBlock.name, filename);

	disc.file = fopen(disc.superBlock.name, "wb+");
	if (!disc.file) return 0;
	unsigned long adress = 0;

	fwrite(&disc.superBlock, sizeof(SuperBlock), 1, disc.file);

	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node = NewNodeInstance();
		fwrite(&node, sizeof(Node), 1, disc.file);
	}

	for (int i = 0; i < disc.superBlock.blocksNumber; ++i)
	{
		int j = UNUSED;
		fwrite(&j, sizeof(int), 1, disc.file);
	}
	return 1;
}

void CloseDiscFile()
{
	if (disc.file)
	{
		fclose(disc.file);
	}
}

void DeleteDisc(char* filename)
{
	if (remove(filename) == 0)
	{
		printf("Successfully deleted disc: %s\n", filename);
	}
	else
	{
		printf("Cannot delete disc: %s\n", filename);
	}
}

unsigned long GetFileSize(FILE *file)
{
	fseek(file, 0L, SEEK_END);
	unsigned long size = ftell(file);
	fseek(file, 0, 0);
	return size;
}

unsigned long GetFileSizeOnDisc(char* filename)
{
	unsigned long adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);

		if (node.size != UNUSED && (strcmp(filename, node.filename) == 0))
			return node.size;

		adress += sizeof(Node);
	}
}

int isFileExistsOnDisc(char* filename)
{
	unsigned long adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);
		if (node.size != UNUSED && (strcmp(filename, node.filename) == 0)) return 1;
		adress += sizeof(Node);
	}
	return 0;
}

long GetFirstBlockIndexOfFile(char* filename)
{
	unsigned long adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);

		if (node.size != UNUSED && (strcmp(filename, node.filename) == 0))
			return node.firstBlock;

		adress += sizeof(Node);
	}
}

void CopyToDisc(char* filename)
{
	FILE* copyFile = fopen(filename, "rb");
	if (!copyFile)
	{
		printf("ERROR: cannot open file %s\n", filename);
		return;
	}

	unsigned long copyFileSize = GetFileSize(copyFile);
	if (copyFileSize > disc.superBlock.freeBlocksNumber * BLOCK_SIZE)
	{
		printf("ERROR: not enough space on disc to copy file %s\n", filename);
		return;
	}

	if (disc.superBlock.freeNodesNumber <= 0)
	{
		printf("ERROR: disc reached max number of files\n");
		return;
	}

	if (isFileExistsOnDisc(filename))
	{
		printf("ERROR: file %s already exists on disc\n", filename);
		return;
	}

	int blockIndex = GetFirstFreeBlockIndex();

	int nodeIndex = GetFirstFreeNodeIndex();
	Node node;
	node.size = copyFileSize;
	node.firstBlock = blockIndex;
	strcpy(node.filename, filename);

	fseek(disc.file, disc.superBlock.nodesOffset + (nodeIndex * sizeof(Node)), 0);
	fwrite(&node, sizeof(Node), 1, disc.file);

	int copyFileSizeInBlocks = copyFileSize / BLOCK_SIZE;
	if (copyFileSize%BLOCK_SIZE != 0) copyFileSizeInBlocks += 1;

	for (int i = 0; i < copyFileSizeInBlocks; ++i)
	{
		Block block;
		fread(block.data, BLOCK_SIZE, 1, copyFile);
		SetBlockState(blockIndex, USED);
		int nextBlockIndex = GetFirstFreeBlockIndex();
		if ((i + 1) == copyFileSizeInBlocks) nextBlockIndex = disc.superBlock.blocksNumber + 1;
		block.nextBlock = nextBlockIndex;
		fseek(disc.file, disc.superBlock.blocksOffset + (blockIndex * sizeof(Block)), 0);
		fwrite(&block, sizeof(Block), 1, disc.file);
		blockIndex = nextBlockIndex;
	}

	disc.superBlock.freeNodesNumber -= 1;
	disc.superBlock.freeBlocksNumber -= copyFileSizeInBlocks;
	fseek(disc.file, 0, 0);
	fwrite(&disc.superBlock, sizeof(SuperBlock), 1, disc.file);

	fclose(copyFile);

	printf("file %s successfully copied to disc\n", filename);
}

void CopyFromDisc(char* filename)
{
	if (!isFileExistsOnDisc(filename))
	{
		printf("ERROR: no such file: %s on disc\n", filename);
		return;
	}

	FILE* destinationFile = fopen(filename, "wb+");
	if (!destinationFile)
	{
		printf("ERROR: cannot create file %s\n", filename);
		return;
	}

	long size = GetFileSizeOnDisc(filename);
	long blockIndex = GetFirstBlockIndexOfFile(filename);
	Block block;

	do
	{
		fseek(disc.file, disc.superBlock.blocksOffset + (blockIndex * sizeof(Block)), 0);
		fread(&block, sizeof(Block), 1, disc.file);
		size -= BLOCK_SIZE;

		if (size < 0) fwrite(block.data, BLOCK_SIZE + size, 1, destinationFile);
		else fwrite(block.data, BLOCK_SIZE, 1, destinationFile);

		blockIndex = block.nextBlock;
	} while (blockIndex != disc.superBlock.blocksNumber + 1);

	fclose(destinationFile);
	printf("file %s successfully copied from disc\n", filename);
}

void DeleteFileFromDisc(char* filename)
{
	if (!isFileExistsOnDisc(filename))
	{
		printf("ERROR: no such file: %s on disc\n", filename);
		return;
	}

	long blockIndex = GetFirstBlockIndexOfFile(filename);
	long sizeInBlocks = 0;

	do
	{
		Block block;
		fseek(disc.file, disc.superBlock.blocksOffset + (blockIndex * sizeof(Block)), 0);
		fread(&block, sizeof(Block), 1, disc.file);

		SetBlockState(blockIndex, UNUSED);

		blockIndex = block.nextBlock;
		++sizeInBlocks;
	} while (blockIndex != disc.superBlock.blocksNumber + 1);

	DeleteNode(filename);

	disc.superBlock.freeNodesNumber += 1;
	disc.superBlock.freeBlocksNumber += sizeInBlocks;
	fseek(disc.file, 0, 0);
	fwrite(&disc.superBlock, sizeof(SuperBlock), 1, disc.file);

	printf("file %s successfully deleted from disc\n", filename);
}

void ListDisc()
{
	if (disc.superBlock.freeNodesNumber == MAX_FILES_NUMBER)
		return; // no files on disc

	unsigned long adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);
		if (node.size != UNUSED) {
			printf("%s\n", node.filename);
		}
		adress += sizeof(Node);
	}
}

void PrintDiscMap()
{
	unsigned long adress = 0;

	SuperBlock superBlock;
	fseek(disc.file, adress, 0);
	fread(&superBlock, sizeof(SuperBlock), 1, disc.file);
	printf("[%lu]\t%s\t%lu\t%d\n", adress, "SuperBlock", sizeof(SuperBlock), USED);

	adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);
		int used = node.size != UNUSED ? 1 : 0;
		printf("[%lu]\t%s\t\t%lu\t%d\n", adress, "Node", sizeof(Node), used);
		adress += sizeof(Node);
	}

	printf("[%lu]\t%s\t%lu\t%d\n", adress, "BitVector", disc.superBlock.blocksNumber * sizeof(int), 1);

	adress = disc.superBlock.bitVectorOffset;
	for (int i = 0; i < disc.superBlock.blocksNumber; ++i)
	{
		int used;
		fseek(disc.file, adress, 0);
		fread(&used, sizeof(int), 1, disc.file);
		if (used == UNUSED) used = 0;
		printf("[%lu]\t%s\t\t%lu\t%d\n", disc.superBlock.blocksOffset + (i * sizeof(Block)), "Block", sizeof(Block), used);
		adress += sizeof(int);
	}
}

