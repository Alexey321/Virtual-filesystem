
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "SuperBlock.h"
#include "Disc.h"
#include "Node.h"



SuperBlock NewSuperBlockInstance(unsigned long size)
{
	SuperBlock superBlock;
	superBlock.blocksNumber = (size - MAX_FILES_NUMBER * sizeof(Node) - sizeof(SuperBlock)) /
		(sizeof(Block) + sizeof(char));
	superBlock.freeBlocksNumber = superBlock.blocksNumber;
	superBlock.freeNodesNumber = MAX_FILES_NUMBER;

	superBlock.nodesOffset = sizeof(SuperBlock);
	superBlock.bitVectorOffset = superBlock.nodesOffset + MAX_FILES_NUMBER * sizeof(Node);
	superBlock.blocksOffset = superBlock.bitVectorOffset + superBlock.blocksNumber * sizeof(char);

	return superBlock;
}


int GetFirstFreeBlockIndex()
{
	unsigned long adress = disc.superBlock.bitVectorOffset;
	char bit;  //char bit, i = 0;
	int i = 0; // if there is char, files would be corrupted

	do
	{
		fseek(disc.file, adress, 0);
		fread(&bit, sizeof(char), 1, disc.file);
		adress += sizeof(char);
		++i;
	} while (bit == USED);

	return i - 1;
}

void SetBlockState(int index, int state)
{
	unsigned long adress = disc.superBlock.bitVectorOffset + (index * sizeof(char));
	char bit = state;
	fseek(disc.file, adress, 0);
	fwrite(&bit, sizeof(char), 1, disc.file);
}

int GetFirstFreeNodeIndex()
{
	unsigned long adress = disc.superBlock.nodesOffset;
	Node node = NewNodeInstance();
	int i = 0;

	do
	{
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);
		adress += sizeof(Node);
		++i;
	} while (node.size != UNUSED);

	return i - 1;
}

void DeleteNode(char* filename)
{
	unsigned long adress = disc.superBlock.nodesOffset;
	for (int i = 0; i < MAX_FILES_NUMBER; ++i)
	{
		Node node;
		fseek(disc.file, adress, 0);
		fread(&node, sizeof(Node), 1, disc.file);

		if (node.size != UNUSED && (strcmp(filename, node.filename) == 0))
		{
			node.size = UNUSED;
			fseek(disc.file, adress, 0);
			fwrite(&node, sizeof(Node), 1, disc.file);
			return;
		}

		adress += sizeof(Node);
	}
}