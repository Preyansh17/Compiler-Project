#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parser.h"
#include "stack.h"
#include "NaryTree.h"
#include "hashTable.h"
#include "lexer.h"

char* non_terminals2[] = {"actualOrRedefined",
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

int initialized_lexer = 0, initialized_look_up = 0;


char* keywords2[] = {NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
"with",
"parameters",
"end",
"while",
"union",
"endunion",
"definetype",
"as",
"type",
"_main",
"global",
"parameter",
"list",
NULL,
NULL,
"input",
"output",
"int",
"real",
NULL,
NULL,
NULL,
NULL,
"endwhile",
NULL,
NULL,
"if",
"then",
"endif",
"read",
"write",
"return",
NULL,
NULL,
NULL,
NULL,
"call",
"record",
"endrecord",
"else",
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL};

char* tokenMap2[] = {"TK_ASSIGNOP",
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
"eps", "$", "TK_ERROR"
};
int created_parse_table = 0, root_node_created = 0, grammar_file_closed = 0, break_loop = 0;
;

int checkCompilerInputError(int numArg) {
	if (numArg == 3)
		return 0;
	else 
		return 1;
}

int initialized_plook_up = 0;

void initialize_plook_up() {
	initialized_plook_up = 1;
	// plookupTable = createLookUpTable(NUMBER_OF_SLOTS);
   plookupTable = (pTable*)malloc(sizeof(pTable));
      plookupTable->entries = (pEntry*) malloc(NUMBER_OF_SLOTS * sizeof(pEntry));
      plookupTable->m = NUMBER_OF_SLOTS;
	plookupTable = PopulateLookUpTable(plookupTable, non_terminals2, TOTAL_NON_TERMINALS, NUMBER_OF_SLOTS);
	plookupTable = PopulateLookUpTable(plookupTable, tokenMap2, TOTAL_TERMINALS, NUMBER_OF_SLOTS);
}

int main(int argc, char* argv[]){

	if(checkCompilerInputError(argc)){
		printf("Check command: ./compiler testcase(x).txt outputParseTree.txt\n");
		return 0;
	}

	char test_file_name[128];
	strcpy(test_file_name, argv[1]);

	double total_CPU_time;
	double total_CPU_time_in_seconds;
	clock_t start_time;
	clock_t end_time;
  

	printf("***************************Information of implementation details:***************************\n\n"
	"(a) FIRST and FOLLOW set automated\n"
	"(b) Both lexical and syntax analysis modules implemented\n"
	"(c) Module works with all given testcases\n"
	"(d) Parse tree constructed\n\n"
	"***************************End of implementation details***************************\n\n");
	//printf("(a) FIRST and FOLLOW set automated\n");
	// printf("(b) Only Lexical Analyser module developed\n");
	//printf("(b) Both lexical and syntax analysis modules implemented\n");
	// printf("(d) Modules compile but give segmentation fault\n");
	//printf("(c) Module works with all given testcases\n");
	// printf("(d) Parse tree constructed\n\n");

	//printf("***************************End of implementation details***************************\n\n");

    FILE * fp1 = fopen(argv[1], "a");
    fputs("\n", fp1);
    fclose(fp1);

	printf("----------------------------- Execution started -----------------------------\n");

	if (initialized_lexer == 0){
		initializeLexer();
		initialized_lexer = 1;
	}
	/*************************** Interface ***************************/
	while(1) {

    	int choice;

		printf("\nPress option for the defined task:\n\n"
		"0: Exit\n"
		"1: Removal of comments - print the comment free code on the console\n"
		"2: Printing the token list generated by the lexer\n"
		"3: Parsing to verify the syntactic correctness of the input source code and print the parse tree\n"
		"4: Printing the total time taken by our stage 1 code of lexer and parser to verify the syntactic correctness\n"
		"\n\nYour Input: ");

		if(scanf("%d", &choice)) {

		printf("Choice : %d\n", choice); }
		else{
		printf("Failed to read integer.\n");
		break;}
    
		switch(choice){

			case 0:
				break_loop = 1;
				break;

			case 1: {
				if (!initialized_lexer){
					initialized_lexer = 1;
         			initializeLexer();
				}
				char temp_file_name[128];
				strcpy(temp_file_name, test_file_name);
				FILE* test_case = fopen(test_file_name, "r");
				FILE* cleaned = fopen("cleaned_test_file.txt", "w");

        		removeComments(test_file_name, "cleaned_test_file.txt");
				strcpy(test_file_name, temp_file_name);
			}
				break;

			case 2: {
				printf("Lexical Tokens of %s\n", test_file_name);
        		start_time = clock();
				if (!initialized_lexer){
					initialized_lexer = 1;
          			initializeLexer();
				}
				printf("Lexer init\n");
				char temp_file_name[128];
				strcpy(temp_file_name, test_file_name);
				FILE* fp = fopen(temp_file_name, "r");
				unSetOverFlag();
				Token* token;
				while((token = getNextToken(&fp)) != NULL) {
          			if(strcmp(token->lexeme,"%")==0 || strcmp(tokenMap2[token->type],"TK_ERROR")==0)
						continue;
					printf("Line no.%-2d ",token->line_no);
					printf("%*c",2,' ');
					printf("Lexeme %-15s ",token->lexeme);
					printf("%*c",2,' ');
					printf("Token  %s\n",tokenMap2[token->type]);
					if (fp == NULL) {
						printf("File pointer null\n");
					}
				}
				if (fp != NULL)
					fclose(fp);
				printf("Over...");
				strcpy(test_file_name, temp_file_name);
        		end_time = clock();
				total_CPU_time += (double) (end_time - start_time);
				total_CPU_time_in_seconds += total_CPU_time / CLOCKS_PER_SEC;
			}
				break;

			case 3: {
        		int errors;
				char temp_file_name[128];
				strcpy(temp_file_name, test_file_name);
				unSetOverFlag();
				start_time = clock();
				if (!initialized_lexer) {
					initialized_lexer = 1;
          			initializeLexer();
				}
				if (!initialized_plook_up) {
					initialize_plook_up();
				}
				FirstAndFollow* first_and_follow = ComputeFirstAndFollowSets();
				
				printf("*************************First Sets************************* \n\n\n");
				for(int i = 0; i < TOTAL_NON_TERMINALS; i++) {
					printf("%s = {", non_terminals2[i]);
					for(int j = 0; j < TOTAL_TERMINALS; j++) {
						if (first_and_follow->first[i][j] > 0) {
							printf("%s,", tokenMap2[j]);
						}
					}
					printf ("}\n");
				}
				printf("*************************Follow Sets************************* \n\n\n");
				for(int i = 0; i < TOTAL_NON_TERMINALS; i++) {
					printf("%s = {", non_terminals2[i]);
					for(int j = 0; j < TOTAL_TERMINALS; j++) {
						if (first_and_follow->follow[i][j] == 1) {
							printf("%s,", tokenMap2[j]);
						}
					}
					printf ("}\n");
				}
				if (!created_parse_table){
          			created_parse_table = 1;
					parse_table = createParseTable(first_and_follow);
				}
				// printParseTable(parse_table);
				
				FILE* outputfile = fopen(argv[2], "w");
				fprintf(outputfile, "%-10s %-5s %-13s %-5s %-20s %-9s %s\n\n\n", "LEXEME","LINE","TOKEN","VALUE","PARENT","IS LEAF?","NODE SYMBOL");

				// if (!root_node_created){
					root_node_created = 1;
					rootNode = parseInputSourceCode(temp_file_name, parse_table, &errors);
				// 
				
				printParseTree(rootNode, tokenMap2, non_terminals2, outputfile);

				fclose(outputfile);
				// printf("-------Parsing Table Created-------\n\n");
				strcpy(test_file_name, temp_file_name);
				end_time = clock();
				total_CPU_time += (double) (end_time - start_time);
				total_CPU_time_in_seconds += total_CPU_time / CLOCKS_PER_SEC;
			}
				break;

			case 4:
				printf("Total CPU time : %lf\n", total_CPU_time);
				printf("Total CPU time in seconds : %lf\n", total_CPU_time_in_seconds);
				break;

      		default:
				printf("Incorrect Option, Please Enter the correct choice\n");
				break;
		}

		if(break_loop == 0) {
			continue;
    }
    else
      break;

    }
}