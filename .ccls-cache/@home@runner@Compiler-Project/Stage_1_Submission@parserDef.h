/*
Group:33
2016A7PS0036P Megh Thakkar
2016A7PS0103P Sahil Singla
2016A7PS0110P Sankalp Sangle
2016A7PS0150P Patel Parth
*/

#ifndef PARSERDEF
#define PARSERDEF

#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "NaryTree.h"

#define TOTAL_NON_TERMINALS 53
#define TOTAL_TERMINALS 59
#define NO_OF_RULES 95

#define MAX_SYMBOL_SIZE 51

#define BIT_VECTOR_SIZE (sizeof(int)-1+ TOTAL_TERMINALS)/sizeof(int)

//2D array for first and follow sets
typedef struct {
	int ** first;
	int ** follow;
} FirstAndFollow; 

typedef int** parseTable;
parseTable parse_table;
parseTreeNode* rootNode;
#endif