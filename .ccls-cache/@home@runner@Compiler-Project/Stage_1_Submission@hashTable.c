#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

int hashValue(char* str, int m) {
    int accumulator = 0;
    int length = strlen(str);
    int i = 1;
    while(i <= length) {
        accumulator = accumulator * 97 + (str[i-1] - ' ');
        if(accumulator < 0) {
            accumulator = 96;
        }
        i++;
    }
    accumulator %= m;
    return accumulator;
}
Table* initialiseTable(int no_of_slots){
  Table* table = (Table*)malloc(sizeof(Table));
    table->entries = (Entry*)malloc(no_of_slots * sizeof(Entry));
    table->m = no_of_slots;
  return table;
}

Node* initialiseNode(int i){
  Node* n = (Node*)malloc(sizeof(Node));
            n->tType = i-1;
            n->lexeme = (char*)malloc(sizeof(char) * 50);
            n->next = NULL;
  return n;
}

Table* createAndPopulateLookUpTable(char** tokens, int no_of_tokens, int no_of_slots) {
    Table* table = initialiseTable(no_of_slots);  
    int i = 1;
    while(1) {
        if(i > no_of_tokens) {
            break;
        }
        if(tokens[i-1]) {
            Node* n = initialiseNode(i);

          strcpy(n->lexeme, tokens[i-1]);
            int hashed_value = hashValue(tokens[i-1], no_of_slots);

            if(table->entries[hashed_value].head) {
                n->next = table->entries[hashed_value].head;
                table->entries[hashed_value].head = n;
            }
            else {
                table->entries[hashed_value].head = n;
            }
            // printf("%s %d %d\n  ", n->lexeme, n->tType, hashed_value);
            i++;
        }
        else {
            i++;
            continue;
        }
    }
    return table;
}


int lookUp(char* lex) {
    int hashedValue = hashValue(lex, lookUpTable->m);
    Node* tmp = lookUpTable->entries[hashedValue].head;
    while(1) {
        if(tmp) {
            if(strcmp(lex, tmp->lexeme) == 0) {
                return tmp->tType;
            }
            tmp = tmp->next;
        }
        else {
            break;
        }
    }
    return -1;
}

// pTable* createLookUpTable(int no_of_slots)
// {
//       pTable* table = (pTable*)malloc(sizeof(pTable));
//       table->entries = (pEntry*) malloc(no_of_slots * sizeof(pEntry));
//       table->m = no_of_slots;
//     return table;
// }
pNode* initialisepNode(int i){
  pNode* n = (pNode*)malloc(sizeof(pNode));
            n->tType = i-1;
            n->lexeme = (char*)malloc(sizeof(char) * 51);
           
            n->next = NULL;
  return n;
}

pTable* PopulateLookUpTable(pTable* table,char** tokens,int no_of_tokens, int no_of_slots) {
    
    int i = 1;
    while(1) {
        if(i > no_of_tokens) {
            break;
        }
        if(tokens[i-1]) {
            pNode* n = initialisepNode(i);

           strcpy(n->lexeme, tokens[i-1]);
            int hashed_value = hashValue(tokens[i-1], no_of_slots);

            if(table->entries[hashed_value].head) {
                n->next = table->entries[hashed_value].head;
                table->entries[hashed_value].head = n;
            }
            else{
                table->entries[hashed_value].head = n;
            }
            i++;
        }
        else {
            i++;
            continue;
        }
    }
  
    return table;
}

int pLookUp(pTable* lookupTable,char* lex) {
    int hashedValue = hashValue(lex, lookupTable->m);
    pNode* tmp = lookupTable->entries[hashedValue].head;
    while(1) {
        if(tmp) {
            if(!strcmp(lex, tmp->lexeme)) {
                // printf("returned type is %s", tokenMap[tmp->tType]);
                return tmp->tType;
            }
            tmp = tmp->next;
        }
        else {
            break;
        }
    }
  //printf("%s ", tmp->lexeme);
  printf("%s\n", lex);
    return -1;
}