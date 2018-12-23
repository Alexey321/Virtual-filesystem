#pragma once

#define FILENAME_LENGTH 16

typedef struct
{
	long size;
	unsigned long firstBlock;
	char filename[FILENAME_LENGTH];
} Node;

Node NewNodeInstance();

