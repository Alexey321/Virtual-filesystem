#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SuperBlock.h"

#define MAX_FILES_NUMBER 200

typedef struct
{
	SuperBlock superBlock;
	FILE* file;
} Disc;

Disc disc;

Disc NewDiscInstance(unsigned int size);
int OpenDiscFile(char* filename);
int CreateDiscFile(char* filename);
void CloseDiscFile();
void DeleteDisc(char* filename);
unsigned long GetFileSize(FILE *file);
unsigned long GetFileSizeOnDisc(char* filename);
int isFileExistsOnDisc(char* filename);
long GetFirstBlockIndexOfFile(char* filename);
void CopyToDisc(char* filename);
void CopyFromDisc(char* filename);
void DeleteFileFromDisc(char* filename);
void ListDisc();
void PrintDiscMap();