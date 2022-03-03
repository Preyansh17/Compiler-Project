/*
Group:33
2016A7PS0036P Megh Thakkar
2016A7PS0103P Sahil Singla
2016A7PS0110P Sankalp Sangle
2016A7PS0150P Patel Parth
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "stackDef.h"

#include "parserDef.h"
#include "NaryTree.h"

typedef parseTreeNode stackData;

struct StackNode{
	stackData* stack_data;
	struct StackNode* next;
};

typedef struct StackNode StackNode;

typedef struct {
  StackNode* top;
	StackNode* bottom;
	int size;
} Stack;

Stack* createStack();
// StackNode* createStackNode(stackData* stack_data);
StackNode* pop(Stack* stack);
StackNode* top(Stack* stack);
void push(Stack* stack, stackData* stack_data);
void pushAllChildrenOnStack(Stack* stack, stackData* stack_data);
void printStack(Stack* stack);