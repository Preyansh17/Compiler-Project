/*
Group:33
2016A7PS0036P Megh Thakkar
2016A7PS0103P Sahil Singla
2016A7PS0110P Sankalp Sangle
2016A7PS0150P Patel Parth
*/

#ifndef NARYTREE
#define NARYTREE

//#include "NaryTreeDef.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "hashTable.h"

typedef enum{
	actualOrRedefined,
	arithmeticExpression,
	assignmentStmt,
	booleanExpression,
	conditionalStmt,
	constructedDatatype,
	dataType,
	declaration,
	declarations,
	elsePart,
	expPrime,
	factor,
	fieldDefinition,
	fieldDefinitions,
	fieldtype,
	funCallStmt,
	function,
	global_or_not,
	highPrecedenceOperator,
	idList,
	input_par,
	inputParameters,
	ioStmt,
	iterativeStmt,
	logicalOp,
	lowPrecedenceOperators,
	mainFunction,
	more_ids,
	moreExpansions,
	moreFields,
	oneExpansion,
	option_single_constructed,
	optionalReturn,
	otherFunctions,
	otherStmts,
	output_par,
	outputParameters,
	parameter_list,
	primitiveDatatype,
	program,
	relationalOp,
	remaining_list,
	returnStmt,
	singleOrRecId,
	stmt,
	stmts,
	term,
	termPrime,
	typeDefinition,
	typeDefinitions,
	var,
  definetypestmt,
  A,
} NonTerminal;

typedef union {
	Tokentype terminalType;
	NonTerminal nonTerminalType;
} symbolType;

struct parseTreeNode{
    int isTerminal;
    symbolType symType;
    struct parseTreeNode* nextSibling;
    Token* token;
    struct parseTreeNode* parent;
    int no_children;
    struct parseTreeNode* children;
};

typedef struct parseTreeNode parseTreeNode;

parseTreeNode* createNode(int isTerminal, int type, parseTreeNode* parent, Token* token);
parseTreeNode* initializeParseTree();
parseTreeNode* addChildren(char* rule, parseTreeNode* parent);
void printParseTree(parseTreeNode* root, char** tokenMap, char** nonterminals, FILE* outputfile);
void printNode(FILE* outputfile,parseTreeNode* node, char** tokenMap, char** non_terminals, int isLeaf, char* empty, char* no, char* yes, char* rootMessage );

#endif