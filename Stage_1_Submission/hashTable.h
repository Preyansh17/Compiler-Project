#ifndef HASHTABLE
#define HASHTABLE

// #include "hashTableDef.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"

#define NUMBER_OF_SLOTS 500

struct node{
    Tokentype tType;
    char * lexeme;
    struct node* next;
};

typedef struct node Node;

typedef struct {
    struct node * head;
} Entry;



typedef struct {
    int m; // no of slots in hash table
    Entry* entries; 
} Table;

Table* lookUpTable;

struct pNode{
    int tType;
    char* lexeme;
    struct pNode* next;
};

typedef struct pNode pNode;

typedef struct {
    struct pNode* head;
} pEntry;

typedef struct {
    int m; // no of slots in hash table
    pEntry* entries; 
} pTable;

pTable* plookupTable;

int hashValue(char* str, int m);
Table* createAndPopulateLookUpTable(char** tokens, int no_of_tokens, int no_of_slots);
int lookUp(char* lex);
pTable* createLookUpTable(int no_of_slots);
pTable* PopulateLookUpTable(pTable* table,char** tokens,int no_of_tokens, int no_of_slots);
int pLookUp(pTable* lookupTable,char* lex);



 #endif