#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "parserDef.h"

char* error = "(ERROR)";

parseTreeNode* createNode(int isTerminal, int type, parseTreeNode* parent, Token* token) {
    parseTreeNode * p = (parseTreeNode*) malloc(sizeof(parseTreeNode));
    p->children = NULL;
    p->no_children = 0;
    p->isTerminal = isTerminal;
    switch (isTerminal)
    {
    case 0:
        p->symType.nonTerminalType = (NonTerminal) type;
        break;
    
    case 1:
        p->symType.terminalType = (Tokentype) type;
        break;
    }
    p->nextSibling = NULL;   
    p->parent = parent;
    p->token = token;

    return p;
}

int isToken(char * word) {
    return (word[0] == 'T' && word[1] == 'K') ? 1 : 0;
}

int isEpsilon(char * word) {
    return (strcmp("eps", word) == 0) ? 1 : 0;
}

int isNull(char * word) {
    return (word == NULL) ? 1 : 0;
}

parseTreeNode* addChildren(char* rule, parseTreeNode* parent) {
    
    char *NT = strstr(rule, "> ");
    NT += 2;

    char *right_NT = strdup(NT);
    char* word = (char*)malloc(sizeof(char)*20);
    
    parseTreeNode* prev, *next, *head;
    prev = NULL; next = NULL;
    while(1) {
        word = strtok_r(right_NT, " ", &right_NT);
        
        if(isNull(word))
            return head;
        
        if(isEpsilon(word))
            return NULL;
        
        switch (isToken(word))
        {
        case 0:
            next = createNode(0, pLookUp(plookupTable, word), parent, NULL);
            if(!(prev)) 
              {
                prev = next;
                head = next;
            }
            else 
            {
                prev->nextSibling = next;
                prev = next;
            }
            break;
        
        case 1:
            next = createNode(1, pLookUp(plookupTable, word), parent, NULL);
            if(!(prev)) 
              {
                prev = next;
                head = next;
            }
            else 
            {
                prev->nextSibling = next;
                prev = next;
            }
            break;
        }
    }
}

void printNode(FILE* outputfile,parseTreeNode* node, char** tokenMap, char** non_terminals, int isLeaf, char* empty, char* no, char* yes, char* rootMessage){

	//Leaf Node
    switch(isLeaf) {
        
        case 0:
            //Root Node
            if(node->parent == NULL)
                // printLine(outputfile, empty, 0, empty, 0, 0, 0, 0, rootMessage,no,non_terminals[node->symType.nonTerminalType]);
                fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s <%s>\n", empty,empty,empty,empty,rootMessage,no,non_terminals[node->symType.nonTerminalType]);
            else
                fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s <%s>\n", empty,empty,empty,empty,non_terminals[node->parent->symType.nonTerminalType],no,non_terminals[node->symType.nonTerminalType]);
            break;
        
        case 1:
            //If leaf node is a non-terminal (in-case of incomplete trees) -- ERROR CASE
            if(!(node->isTerminal)){
                
                //Root Node
                if(node->parent)
                  {
                    fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s <%s>\n", empty,empty,empty,empty,non_terminals[node->parent->symType.nonTerminalType],yes,non_terminals[node->symType.nonTerminalType]);

                }
                else
              {
                    fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s <%s>\n", empty,empty,empty,empty,rootMessage,yes,non_terminals[node->symType.nonTerminalType]);
                }
            }	
              //Some terminal nodes may not be assigned lexical tokens since they are not matched -- ERROR CASE
            else if(node->token == NULL){
                            fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s %s\n", error, error, error, empty, non_terminals[node->parent->symType.nonTerminalType], yes, tokenMap[node->symType.terminalType]);
                        }
            //No lexeme for epsilon
            else if(node->symType.terminalType == EPSILON){
                fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s %s\n", empty,empty,empty,empty,non_terminals[node->parent->symType.nonTerminalType],yes,tokenMap[node->symType.terminalType]);
            }

            
            
            
            else
                switch (node->token->vtype)
                {
                case -1:
                    fprintf(outputfile, "%-10s %-5d %-13s %-5s %-20s %-9s %s\n", node->token->lexeme, node->token->line_no, tokenMap[node->token->type], empty, non_terminals[node->parent->symType.nonTerminalType], yes, tokenMap[node->symType.terminalType]);
                    break;
                case 0:
                    fprintf(outputfile, "%-10s %-5d %-13s %-5d %-20s %-9s %s\n", node->token->lexeme, node->token->line_no, tokenMap[node->token->type], node->token->value->num, non_terminals[node->parent->symType.nonTerminalType], yes, tokenMap[node->symType.terminalType]);
                    break;
                case 1:
                    fprintf(outputfile, "%-10s %-5d %-13s %-5f %-20s %-9s %s\n", node->token->lexeme, node->token->line_no, tokenMap[node->token->type], node->token->value->r_num, non_terminals[node->parent->symType.nonTerminalType], yes, tokenMap[node->symType.terminalType]);
                    break;
                }
            break;
    }
}

void printParseTreeUtility(parseTreeNode* root, char** tokenMap, char** non_terminals, FILE* outputfile) {
  if(root==NULL)
    return;

    int isLeaf;

    if(root->children != NULL) {
        isLeaf = 0;
        printParseTreeUtility(root->children, tokenMap, non_terminals, outputfile); 
    }

    if (root->children == NULL)
        isLeaf = 1;

    printNode( outputfile,root, tokenMap, non_terminals, isLeaf,  "----", "no", "yes", "ROOT");
    
    parseTreeNode* tmp = root->children;
    if(tmp != NULL) {
        tmp = tmp -> nextSibling;
        for(; tmp!=NULL;)
        {
            printParseTreeUtility(tmp, tokenMap, non_terminals, outputfile);
            tmp = tmp -> nextSibling;
        }
    }
}