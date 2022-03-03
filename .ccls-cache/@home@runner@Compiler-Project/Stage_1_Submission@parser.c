 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "stack.h"
#include "NaryTree.h"
#include "hashTable.h"
#include "lexer.h"

int no_of_rules = 95;

char* tokenMap[] = {"TK_ASSIGNOP",
"TK_COMMENT",
"TK_FIELDID",
"TK_ID",
"TK_NUM",
"TK_RNUM",
"TK_FUNID",
"TK_RUID",
"TK_WITH",
"TK_PARAMETERS",
"TK_END",
"TK_WHILE",
"TK_UNION",
"TK_ENDUNION",
"TK_DEFINETYPE",
"TK_AS",
"TK_TYPE",
"TK_MAIN",
"TK_GLOBAL",
"TK_PARAMETER",
"TK_LIST",
"TK_SQL",
"TK_SQR",
"TK_INPUT",
"TK_OUTPUT",
"TK_INT",
"TK_REAL",
"TK_COMMA",
"TK_SEM",
"TK_COLON",
"TK_DOT",
"TK_ENDWHILE",
"TK_OP",
"TK_CL",
"TK_IF",
"TK_THEN",
"TK_ENDIF",
"TK_READ",
"TK_WRITE",
"TK_RETURN",
"TK_PLUS",
"TK_MINUS",
"TK_MUL",
"TK_DIV",
"TK_CALL",
"TK_RECORD",
"TK_ENDRECORD",
"TK_ELSE",
"TK_AND",
"TK_OR",
"TK_NOT",
"TK_LT",
"TK_LE",
"TK_EQ",
"TK_GT",
"TK_GE",
"TK_NE",    
"eps", 
"$", 
"TK_ERROR"};

char* non_terminals[] = {"actualOrRedefined",
"arithmeticExpression",
"assignmentStmt",
"booleanExpression",
"conditionalStmt",
"constructedDatatype",
"dataType",
"declaration",
"declarations",
"elsePart",
"expPrime",
"factor",
"fieldDefinition",
"fieldDefinitions",
"fieldtype",
"funCallStmt",
"function",
"global_or_not",
"highPrecedenceOperator",
"idList",
"input_par",
"inputParameters",
"ioStmt",
"iterativeStmt",
"logicalOp",
"lowPrecedenceOperators",
"mainFunction",
"more_ids",
"moreExpansions",
"moreFields",
"oneExpansion",
"option_single_constructed",
"optionalReturn",
"otherFunctions",
"otherStmts",
"output_par",
"outputParameters",
"parameter_list",
"primitiveDatatype",
"program",
"relationalOp",
"remaining_list",
"returnStmt",
"singleOrRecId",
"stmt",
"stmts",
"term",
"termPrime",
"typeDefinition",
"typeDefinitions",
"var",
"definetypestmt",
"A"};

char* grammar[NO_OF_RULES] = {
    "program ===> otherFunctions mainFunction",
"mainFunction ===> TK_MAIN stmts TK_END",
"otherFunctions ===> function otherFunctions",
"otherFunctions ===> eps",
"function ===> TK_FUNID input_par output_par TK_SEM stmts TK_END",
"input_par ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR",
"output_par ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR",
"output_par ===> eps",
"parameter_list ===> dataType TK_ID remaining_list",
"dataType ===> primitiveDatatype",
"dataType ===> constructedDatatype",
"primitiveDatatype ===> TK_INT",
"primitiveDatatype ===> TK_REAL",
"constructedDatatype ===> TK_RECORD TK_RUID",
"constructedDatatype ===> TK_UNION TK_RUID",
"constructedDatatype ===> TK_RUID",
"remaining_list ===> TK_COMMA parameter_list",
"remaining_list ===> eps",
"stmts ===> typeDefinitions declarations otherStmts returnStmt",
"typeDefinitions ===> actualOrRedefined typeDefinitions",
"typeDefinitions ===> eps",
"actualOrRedefined ===> typeDefinition",
"actualOrRedefined ===> definetypestmt",
"typeDefinition ===> TK_RECORD TK_RUID fieldDefinitions TK_ENDRECORD",
"typeDefinition ===> TK_UNION TK_RUID fieldDefinitions TK_ENDUNION",
"fieldDefinitions ===> fieldDefinition fieldDefinition moreFields",
"fieldDefinition ===> TK_TYPE fieldtype TK_COLON TK_FIELDID TK_SEM",
"fieldtype ===> primitiveDatatype",
"fieldtype ===> TK_RUID",
"moreFields ===> fieldDefinition moreFields",
"moreFields ===> eps",
"declarations ===> declaration declarations",
"declarations ===> eps",
"declaration ===> TK_TYPE dataType TK_COLON TK_ID global_or_not TK_SEM",
"global_or_not ===> TK_COLON TK_GLOBAL",
"global_or_not ===> eps",
"otherStmts ===> stmt otherStmts",
"otherStmts ===> eps",
"stmt ===> assignmentStmt",
"stmt ===> iterativeStmt",
"stmt ===> conditionalStmt",
"stmt ===> ioStmt",
"stmt ===> funCallStmt",
"assignmentStmt ===> singleOrRecId TK_ASSIGNOP arithmeticExpression TK_SEM",
"singleOrRecId ===> TK_ID option_single_constructed",
"option_single_constructed ===> oneExpansion moreExpansions",
"option_single_constructed ===> eps",
"oneExpansion ===> TK_DOT TK_FIELDID",
"moreExpansions ===> oneExpansion moreExpansions",
"moreExpansions ===> eps",
"funCallStmt ===> outputParameters TK_CALL TK_FUNID TK_WITH TK_PARAMETERS inputParameters TK_SEM",
"outputParameters ===> TK_SQL idList TK_SQR TK_ASSIGNOP",
"outputParameters ===> eps",
"inputParameters ===> TK_SQL idList TK_SQR",
"iterativeStmt ===> TK_WHILE TK_OP booleanExpression TK_CL stmt otherStmts TK_ENDWHILE",
"conditionalStmt ===> TK_IF TK_OP booleanExpression TK_CL TK_THEN stmt otherStmts elsePart",
"elsePart ===> TK_ELSE stmt otherStmts TK_ENDIF",
"elsePart ===> TK_ENDIF",
"ioStmt ===> TK_READ TK_OP var TK_CL TK_SEM",
"ioStmt ===> TK_WRITE TK_OP var TK_CL TK_SEM",
"arithmeticExpression ===> term expPrime",
"expPrime ===> lowPrecedenceOperators term expPrime",
"expPrime ===> eps",
"term ===> factor termPrime",
"termPrime ===> highPrecedenceOperator factor termPrime",
"termPrime ===> eps",
"factor ===> TK_OP arithmeticExpression TK_CL",
"factor ===> var",
"highPrecedenceOperator ===> TK_MUL",
"highPrecedenceOperator ===> TK_DIV",
"lowPrecedenceOperators ===> TK_PLUS",
"lowPrecedenceOperators ===> TK_MINUS",
"booleanExpression ===> TK_OP booleanExpression TK_CL logicalOp TK_OP booleanExpression TK_CL",
"booleanExpression ===> var relationalOp var",
"booleanExpression ===> TK_NOT TK_OP booleanExpression TK_CL",
"var ===> singleOrRecId",
"var ===> TK_NUM",
"var ===> TK_RNUM",
"logicalOp ===> TK_AND",
"logicalOp ===> TK_OR",
"relationalOp ===> TK_LT",
"relationalOp ===> TK_LE",
"relationalOp ===> TK_EQ",
"relationalOp ===> TK_GT",
"relationalOp ===> TK_GE",
"relationalOp ===> TK_NE",
"returnStmt ===> TK_RETURN optionalReturn TK_SEM",
"optionalReturn ===> TK_SQL idList TK_SQR",
"optionalReturn ===> eps",
"idList ===> TK_ID more_ids",
"more_ids ===> TK_COMMA idList",
"more_ids ===> eps",
"definetypestmt ===> TK_DEFINETYPE A TK_RUID TK_AS TK_RUID",
"A ===> TK_RECORD",
"A ===> TK_UNION",
};

FirstAndFollow * createFirstFollow() {
  FirstAndFollow * first_follow_sets = (FirstAndFollow *) malloc(sizeof(FirstAndFollow));
    first_follow_sets->first = (int**) malloc(sizeof(int *)*TOTAL_NON_TERMINALS);
    int i=0;
    while(i<TOTAL_NON_TERMINALS)
    {
        first_follow_sets->first[i] = (int*)malloc(sizeof(int)*TOTAL_TERMINALS);
        int j=0;
        while(j<TOTAL_TERMINALS)
        {
          first_follow_sets->first[i][j] = 0;
          j++;
        }
      i++;
    }
    first_follow_sets->follow = (int**)malloc(sizeof(int*)*TOTAL_NON_TERMINALS);
    i=0;
    while(i<TOTAL_NON_TERMINALS)
    // for(int i=0;i<TOTAL_NON_TERMINALS;i++)
    {
        first_follow_sets->follow[i] = (int*)malloc(sizeof(int)*TOTAL_TERMINALS);
      int j=0;
      while(j<TOTAL_TERMINALS)
        // for(int j=0;j<TOTAL_TERMINALS;j++)
        {
          first_follow_sets->follow[i][j] = 0;
          j++;
        }
      i++;
    }
  return first_follow_sets;
}

FirstAndFollow * ComputeFirstAndFollowSets() {
    FirstAndFollow * first_follow_sets = createFirstFollow();
    // computeFirst(first_follow_sets, plookupTable->m);
  int isChanged = 1;
	
    for(;isChanged!=0;)
		// isChanged = 0;
		isChanged = computeFirstUtil(first_follow_sets->first, plookupTable->m);
    // computeFollow(first_follow_sets, plookupTable->m);
  int idX = pLookUp(plookupTable, "program");
  first_follow_sets->follow[idX][TOTAL_TERMINALS-1]=1;
	// addToSet(first_follow_sets->follow[idX],TOTAL_TERMINALS-1);
	isChanged = 1;
	for(;isChanged!=0;)
		// isChanged = 0;
		isChanged = computeFollowUtil(first_follow_sets->first,first_follow_sets->follow, plookupTable->m);
    for(int i=0;i<TOTAL_NON_TERMINALS;i++)
        first_follow_sets->follow[i][TOTAL_TERMINALS-2] = 0;
  
    return first_follow_sets;
}

void append_sets(int* first_set,int* second_set) {
  int i=0;  
  while(i<TOTAL_TERMINALS){
        if(second_set[i]==1)
            first_set[i]=1;
    i++;}
}

int checkIfChanged(int* first_set,int* second_set) {
  int i=0;  
  while(i<TOTAL_TERMINALS){
        if(second_set[i] == 1 && first_set[i] == 0)
            return 1;
    i++;}
    return 0;
}

int computeFollowUtil(int** first, int** follow, int no_of_slots) {
    int i=0;
    int isChanged = 0;
    int end_flag = 0;
    while(i<TOTAL_NON_TERMINALS)
    {
      int j=0;
        while(j<no_of_rules)
        {
            char* useless = (char*) malloc(sizeof(char)*500);
            char* left_NT = (char*) malloc(sizeof(char)*50);
            end_flag = 0;
            sscanf(grammar[j],"%s %[^\n]", left_NT, useless);

            char *NT = strstr(grammar[j], "> ");
            NT += 2;
            char *right_NT = strdup(NT);
            char* token = (char*)malloc(sizeof(char)*20);
            for(;;)
            {
                token = strtok_r(right_NT, " ", &right_NT);
                if(token==NULL || strcmp(token, "eps") == 0)
                    break;
                if(token[0]=='T'&&token[1]=='K')
                    continue;
                else
                {
                    //non_terminal
                    int idx = pLookUp(plookupTable,token);  
                    if(idx==i)
                    {
                    token = strtok_r(right_NT, " ", &right_NT);
                    int tidx;
                    if(!(token))
                    {
                        tidx = pLookUp(plookupTable,left_NT);
                        
                        if(checkIfChanged(follow[idx],follow[tidx])==1)
                        {
                            append_sets(follow[idx],follow[tidx]);
                            isChanged = 1;
                        }
                    }
                    else if(token[0]=='T'&&token[1]=='K')
                    {
                        tidx = pLookUp(plookupTable,token);
                        if(follow[idx][tidx]!=1)
                        {
                            isChanged=1;
                            follow[idx][tidx]=1;
                        }
                    }
                    else
                    {
                        while(1)
                        {
                            if(!(token))
                            {
                                //add 
                                int tidx = pLookUp(plookupTable,left_NT);
                                if(checkIfChanged(follow[idx],follow[tidx])==1)
                                {   
                                    append_sets(follow[idx],follow[tidx]);
                                    isChanged = 1;
                                }
                                end_flag=1;
                                break;
                            }
                            //get first of the token and take union of both the sets
                          else{
                            int tidx = pLookUp(plookupTable,token);
                            if(checkIfChanged(follow[idx],first[tidx])==1)
                            {
                                append_sets(follow[idx],first[tidx]);
                                isChanged = 1;
                            }
                            // append_sets(follow[idx],first[tidx]);
                            if(first[tidx][TOTAL_TERMINALS-2]!=1)
                                break;
                            token = strtok_r(right_NT, " ", &right_NT);    
                        } }
                        if(end_flag)
                            break;
                    }
                } 
                  else
                      continue;}
            }
          j++;
        }
      i++;
    }
    return isChanged;
}

int computeFirstUtil(int** first, int no_of_slots) {
    int isChanged = 0;
    int i=0;
    while( i<TOTAL_NON_TERMINALS)
    {
      int j=0;
        while(j<no_of_rules) {
            char* left_NT = (char*) malloc(sizeof(char)*50);
            char* useless = (char*) malloc(sizeof(char)*500);
            int idX;
            sscanf(grammar[j],"%s %[^\n]",left_NT, useless);
            
            char *NT = strstr(grammar[j], "> ");
            NT += 2;
            char *right_NT = strdup(NT);
            idX = pLookUp(plookupTable, left_NT);

          char* token = (char*) malloc(sizeof(char)*50);
            // if(idX == i) {
                while(1) {
                    if(idX != i)
                      break;
                    token = strtok_r(right_NT, " ", &right_NT);
                    if(token == NULL) {
                        if(!first[idX][TOTAL_TERMINALS - 2]) {
                            first[idX][TOTAL_TERMINALS - 2]++;
                            isChanged = 1;
                        }
                        break;
                    }
                    else if( strcmp(token, "eps") == 0) {
                        if(first[idX][TOTAL_TERMINALS - 2] != 0) 
                        {
                          break;
                        }
                        else
                        {
                            first[idX][TOTAL_TERMINALS - 2] = 1;
                            isChanged = 1;
                        }
                        break; // Adding epsilon
                    }
                      else if(token[0] == 'T' && token[1] == 'K') {
                        int tidX = pLookUp(plookupTable, token);
                        if(first[idX][tidX] == 0) {
                            first[idX][tidX] = 1;
                            isChanged = 1;
                        }
                        break;
                    }
                    else { // non terminal
                        int tidX = pLookUp(plookupTable, token);
                        // printf("%d\n",tidX);
                        int temp=0;
                        for(int i=0;i<TOTAL_TERMINALS - 2;i++) {
                        if(first[tidX][i] == 1 && first[idX][i] == 0)
                           temp = 1;
                        }
                        if(temp) {
                           isChanged = 1;
                           append_sets(first[idX], first[tidX]);
                        }
                        
                        if(!(first[tidX][TOTAL_TERMINALS - 2]-1)) 
                        // if(first[tidX][TOTAL_TERMINALS - 2] == 1) { //epsilon present in tidX
                             first[idX][TOTAL_TERMINALS - 2] = 0;
                        else
                            break;
                    }
                }
            // }
          // free(left_NT);
          // free(right_NT);
          // free(useless);
          j++;
        }
        i++;
    }
    
    return isChanged;
}

int * getFirsts(char* rule_RHS, FirstAndFollow * firstandfollow) {
    
    int* first = (int *) malloc(sizeof(int)*TOTAL_TERMINALS);
    char* token = (char *) malloc(sizeof(char)*50);
    token = strtok_r(rule_RHS, " ", &rule_RHS);
    int is_eps = 1;
    for (int i=0; i<TOTAL_TERMINALS; i++)
        first[i] = 0;
    for(;token != NULL && is_eps;) {

        append_sets(first, firstandfollow->first[pLookUp(plookupTable, token)]);
        is_eps = firstandfollow -> first[pLookUp(plookupTable, token)][TOTAL_TERMINALS - 2];
        token = strtok_r(rule_RHS, " ", &rule_RHS);
    }
    if (!is_eps) 
        first[TOTAL_TERMINALS - 2] = 0;
     else 
        first[TOTAL_TERMINALS - 2] = 1;
    return first;
}

parseTable createParseTable(FirstAndFollow* firstandfollow) {
    parse_table = (int**) malloc(sizeof(int*)*TOTAL_NON_TERMINALS);
  int i=0;
    while( i<TOTAL_NON_TERMINALS) {
        parse_table[i] = (int*)malloc(sizeof(int)*TOTAL_TERMINALS);
        for (int j=0; j<TOTAL_TERMINALS; j++)
            parse_table[i][j] = -1;
      i++;
    }
  
    for (i=0; i<no_of_rules; i++){
        char* rule = grammar[i];
        char* left_NT = (char*)malloc(sizeof(char)*50);
        char* right_NT = (char*)malloc(sizeof(char)*500);
        char* useless = (char*)malloc(sizeof(char)*6);
      char* token = (char*)malloc(sizeof(char)*50);
        sscanf(grammar[i],"%s %s %[^\n]",left_NT,useless,right_NT);
        int lidX = pLookUp(plookupTable, left_NT);
        if ((right_NT[0] == 'T' && right_NT[1] == 'K') || (strcmp(right_NT, "eps") == 0)){
            token = strtok_r(right_NT, " ", &right_NT);
            int token_idX = pLookUp(plookupTable, token);
            if(token_idX==TOTAL_TERMINALS-2)
            {
              int j=0;
                while(j<TOTAL_TERMINALS)
                {
                    if(firstandfollow->follow[lidX][j]==1)
                    {
                        parse_table[lidX][j] = i;
                    }
                  j++;
                }
                continue;
            }
            parse_table[lidX][token_idX] = i;
            continue;
        } 
          else {
               int* firsts = getFirsts(right_NT, firstandfollow);
                if (firsts[TOTAL_TERMINALS - 2]) {
                    append_sets(firsts, firstandfollow->follow[pLookUp(plookupTable, left_NT)]);
                }
            int j=0;
                while( j<TOTAL_TERMINALS) {
                    if (firsts[j] == 1){
                        parse_table[lidX][j] = i;
                    }
                  j++;
                }
        }
    }
  i = 0;
    while(i<TOTAL_NON_TERMINALS)
    {
        if(firstandfollow->first[i][TOTAL_TERMINALS-2]==0)
        {
          int j=0;
            while(j<TOTAL_TERMINALS)
            {
                if(firstandfollow->follow[i][j]==1 && firstandfollow->first[i][j] == 0)
                parse_table[i][j] = -2;
              j++;
            }
          
        }
i++;
    }
    for(int i=0;i<TOTAL_NON_TERMINALS;i++)
    {
        if(parse_table[i][TK_SEM] == -1)
            parse_table[i][TK_SEM] = -2;
    }
    return parse_table;
}

parseTreeNode *  parseInputSourceCode(char *testcaseFile, int** parseTable, int* errors) {
    int error_found = 0;
    FILE * fp = fopen(testcaseFile, "r");

	if(fp == NULL){
		printf("\n\nFILE NOT FOUND!\n\n");
		return NULL;
	}

	//Initializing ParseTree
  Stack * stack = createStack();
  parseTreeNode * dollar_node =  createNode(1, DOLLAR, NULL, NULL);
	parseTreeNode * parse_tree = createNode(0, program, NULL, NULL);
  push(stack, dollar_node);
	push(stack, parse_tree);

	Tokentype lookahead;

	//Initially get a token from the file
	Token* new_token = getNextToken(&fp);
  int i=0;
	
	while(1){
      StackNode * top_node = top(stack);
        if (new_token == NULL) { // file is over/empty
            // StackNode* top_node = top(stack);
            if ((top_node -> stack_data -> isTerminal == 1) && (top_node -> stack_data -> symType.terminalType == DOLLAR)) {
                printf("Input source code is syntactically correct.\n");
            } else {
              if(i==0)
              {
                printf("\n\nError, no tokens in file\n\n");
		            return NULL;
              }
                *errors = 1;
                printf("Error in parsing. File over but stack not empty.\n");
            }
            break;
        }
        //received a token
         lookahead = new_token->type;

        if(lookahead == TK_COMMENT){
            new_token = getNextToken(&fp);
            continue;
        }

        if(lookahead == TK_ERROR) {
            *errors = 1; // Lexical error
            switch(new_token->vtype) {
              case 5:
                new_token->type = TK_ID;
                lookahead = new_token->type;  
                break;
              case 6:
                new_token->type = TK_FUNID;
                lookahead = new_token->type;
                break;
              case 7:
                new_token->type = TK_RNUM;
                lookahead = new_token->type;
                break;
              default:
                new_token = getNextToken(&fp);
                continue;
            }
        }
        //not a lexical error

        //If top of the stack is $ and input is still left
        if ((top_node -> stack_data -> isTerminal == 1) && (top_node -> stack_data -> symType.terminalType == DOLLAR)) {
            printf("\n\nUnexpected tokens at the end - Stack empty but program still left\n\n");
            printf("\n\n------Input source code is syntactically WRONG - PARSING NOT SUCCESSFUL---------------\n\n");
            *errors = 1;
            break;
		}

        //normal parsing
        StackNode * snode = pop(stack);
        parseTreeNode * p = snode->stack_data;
        
        //if non terminal
        if(!p->isTerminal) {
            int rule_no = parseTable[snode->stack_data->symType.nonTerminalType][lookahead];
            switch(rule_no) {
              case -1:
                *errors = 1;
                push(stack, p);
                new_token = getNextToken(&fp);
                continue;
              case -2:
                *errors = 1;
                // printf("Inside ruleno == -2\n");
                fprintf(stderr, "Line %d: The token of type %s for lexeme %s does not match with the expected token of type %s\n", new_token->line_no, tokenMap[new_token->type], new_token->lexeme, non_terminals[p->symType.nonTerminalType]);
                break;
              default:
				      p->children = addChildren(grammar[rule_no], p);
				//PUSH RHS of the rule on the top of the stack
                if(p->children!= NULL)
				          pushAllChildrenOnStack(stack, p->children);
            }
        }
      else {
          if(lookahead == p->symType.terminalType) {
          p->token = new_token;
          //Get next Lookahead Symbol
          new_token = getNextToken(&fp);
          continue;			
          }
          else {
                  // //error handling
            *errors = 1;
            if(p->symType.terminalType == TK_SEM)
              fprintf(stderr, "Line %d: Semicolon is missing\n", new_token->line_no);
            else
              fprintf(stderr, "Line %d: The token %s for lexeme %s  does not match with the expected token %s\n", new_token->line_no, tokenMap[lookahead], new_token->lexeme, tokenMap[p->symType.terminalType]);
          }
        }
        //if top of the stack is a non terminal
		 
        //free(snode);
        //while loop end
    i=1;
    }
  
  if(fp!=NULL)
		fclose(fp);

	return parse_tree;
}